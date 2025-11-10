import json
import os
import subprocess
import urllib.parse
from typing import Any, IO

import llm_utils


def _to_lsp_request(id: int, method: str, params: dict[str, Any]) -> str:
    request: dict[str, Any] = {"jsonrpc": "2.0", "id": id, "method": method}
    if params:
        request["params"] = params

    content = json.dumps(request)
    header = f"Content-Length: {len(content)}\r\n\r\n"
    return header + content


# Same as a request, but without an id.
def _to_lsp_notification(method: str, params: dict[str, Any]) -> str:
    request: dict[str, Any] = {"jsonrpc": "2.0", "method": method}
    if params:
        request["params"] = params

    content = json.dumps(request)
    header = f"Content-Length: {len(content)}\r\n\r\n"
    return header + content


def _parse_lsp_response(id: int, file: IO[str]) -> dict[str, Any]:
    # Ignore all messages until the response with the correct id is found.
    while True:
        header = {}
        while True:
            line = file.readline().strip()
            if not line:
                break
            key, value = line.split(":", 1)
            header[key.strip()] = value.strip()

        content = file.read(int(header["Content-Length"]))
        response: dict[str, Any] = json.loads(content)
        if "id" in response and response["id"] == id:
            return response


def _path_to_uri(path: str) -> str:
    return "file://" + os.path.abspath(path)


def uri_to_path(uri: str) -> str:
    data = urllib.parse.urlparse(uri)

    assert data.scheme == "file"
    assert not data.netloc
    assert not data.params
    assert not data.query
    assert not data.fragment

    path = data.path
    if path.startswith(os.getcwd()):
        path = os.path.relpath(path, os.getcwd())
    return urllib.parse.unquote(path)  # clangd seems to escape paths.


def is_available(executable: str = os.environ.get("CWHY_LSP", "clangd")) -> bool:
    try:
        clangd = subprocess.run(
            [executable, "--version"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
        return clangd.returncode == 0
    except FileNotFoundError:
        return False


class clangd:
    def __init__(
        self,
        executable: str = os.environ.get("CWHY_LSP", "clangd"),
        working_directory: str = os.getcwd(),
    ) -> None:
        self.id = 0
        self.process = subprocess.Popen(
            [executable],
            text=True,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            cwd=working_directory,
        )
        self.initialize()

    def __del__(self) -> None:
        self.process.terminate()

    def initialize(self) -> dict[str, Any]:
        self.id += 1
        request = _to_lsp_request(self.id, "initialize", {"processId": os.getpid()})
        assert self.process.stdin is not None
        assert self.process.stdout is not None
        self.process.stdin.write(request)
        self.process.stdin.flush()
        return _parse_lsp_response(self.id, self.process.stdout)
        # TODO: Assert there is no error.

    def didOpen(self, filename: str, languageId: str) -> None:
        with open(filename, "r") as file:
            text = file.read()

        notification = _to_lsp_notification(
            "textDocument/didOpen",
            {
                "textDocument": {
                    "uri": _path_to_uri(filename),
                    "languageId": languageId,
                    "version": 1,
                    "text": text,
                }
            },
        )
        assert self.process.stdin is not None
        self.process.stdin.write(notification)
        self.process.stdin.flush()

    def didClose(self, filename: str) -> None:
        notification = _to_lsp_notification(
            "textDocument/didClose", {"textDocument": {"uri": _path_to_uri(filename)}}
        )
        assert self.process.stdin is not None
        self.process.stdin.write(notification)
        self.process.stdin.flush()

    def definition(self, filename: str, line: int, character: int) -> dict[str, Any]:
        self.id += 1
        request = _to_lsp_request(
            self.id,
            "textDocument/definition",
            {
                "textDocument": {"uri": _path_to_uri(filename)},
                "position": {
                    # Things are 0-indexed in LSP.
                    "line": line - 1,
                    "character": character - 1,
                },
            },
        )
        assert self.process.stdin is not None
        assert self.process.stdout is not None
        self.process.stdin.write(request)
        self.process.stdin.flush()
        return _parse_lsp_response(self.id, self.process.stdout)

    def documentSymbol(self, filename: str) -> dict[str, Any]:
        self.id += 1
        request = _to_lsp_request(
            self.id,
            "textDocument/documentSymbol",
            {
                "textDocument": {"uri": _path_to_uri(filename)},
            },
        )
        assert self.process.stdin is not None
        assert self.process.stdout is not None
        self.process.stdin.write(request)
        self.process.stdin.flush()
        return _parse_lsp_response(self.id, self.process.stdout)


def definition_plus_heuristics(filename: str, lineno: int, symbol: str) -> str:
    try:
        with open(filename, "r") as file:
            lines = file.readlines()
    except FileNotFoundError:
        return f"file '{filename}' not found."

    if lineno - 1 >= len(lines):
        return "symbol not found at that location."

    # We care about the end symbol, not namespaces.
    symbol = symbol.split("::")[-1]

    # We just return the first match here. Maybe we should find all definitions.
    character = lines[lineno - 1].find(symbol)

    # Now, some heuristics to make up for GPT's terrible math skills.
    if character == -1:
        symbol = symbol.lstrip("*")
        character = lines[lineno - 1].find(symbol)

    # Check five lines above and below.
    if character == -1:
        for i in range(-5, 6, 1):
            if lineno - 1 + i < 0 or lineno - 1 + i >= len(lines):
                continue
            character = lines[lineno - 1 + i].find(symbol)
            if character != -1:
                lineno += i
                break

    if character == -1:
        return "symbol not found at that location."

    _clangd = clangd()
    _clangd.didOpen(filename, "c" if filename.endswith(".c") else "cpp")
    definition = _clangd.definition(filename, lineno, character + 1)
    _clangd.didClose(filename)

    if "result" not in definition or not definition["result"]:
        return "No definition found."

    path = uri_to_path(definition["result"][0]["uri"])
    start_lineno = definition["result"][0]["range"]["start"]["line"] + 1
    end_lineno = definition["result"][0]["range"]["end"]["line"] + 1
    lines, first = llm_utils.read_lines(path, start_lineno - 5, end_lineno + 5)
    content = llm_utils.number_group_of_lines(lines, first)
    line_string = (
        f"line {start_lineno}"
        if start_lineno == end_lineno
        else f"lines {start_lineno}-{end_lineno}"
    )
    return f"""File '{path}' at {line_string}:\n```\n{content}\n```"""


def document_symbols(filename: str) -> str:
    _clangd = clangd()
    _clangd.didOpen(filename, "c" if filename.endswith(".c") else "cpp")
    data = _clangd.documentSymbol(filename)
    _clangd.didClose(filename)
    if "result" not in data or not data["result"]:
        return "No symbols found."

    def kind_to_string(kind: int) -> str:
        kinds = {
            1: "File",
            2: "Module",
            3: "Namespace",
            4: "Package",
            5: "Class",
            6: "Method",
            7: "Property",
            8: "Field",
            9: "Constructor",
            10: "Enum",
            11: "Interface",
            12: "Function",
            13: "Variable",
            14: "Constant",
            15: "String",
            16: "Number",
            17: "Boolean",
            18: "Array",
            19: "Object",
            20: "Key",
            21: "Null",
            22: "EnumMember",
            23: "Struct",
            24: "Event",
            25: "Operator",
            26: "TypeParameter",
        }
        return kinds.get(kind, f"Unknown({kind})")

    def should_include_symbol(kind: int) -> bool:
        return kind not in {
            8,  # Field
        }

    path = uri_to_path(data["result"][0]["location"]["uri"])
    output = f"Replaceable symbols for '{path}':"
    for symbol in data["result"]:
        if not should_include_symbol(symbol["kind"]):
            continue
        output += f"\n -  {symbol['name']} ({kind_to_string(symbol['kind'])})"
    return output


def source_for_symbol(filename: str, symbol: str) -> str:
    _clangd = clangd()
    _clangd.didOpen(filename, "c" if filename.endswith(".c") else "cpp")
    data = _clangd.documentSymbol(filename)
    _clangd.didClose(filename)
    if "result" not in data or not data["result"]:
        return "No symbols found."

    for sym in data["result"]:
        if sym["name"] == symbol:
            path = uri_to_path(sym["location"]["uri"])
            start_lineno = sym["location"]["range"]["start"]["line"] + 1
            end_lineno = sym["location"]["range"]["end"]["line"] + 1
            lines, first = llm_utils.read_lines(path, start_lineno, end_lineno)
            content = llm_utils.number_group_of_lines(lines, first)
            return f"""File '{path}':\n```\n{content}\n```"""

    return f"Symbol '{symbol}' not found."
