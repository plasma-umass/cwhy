import argparse
import json
import os
from typing import Optional

import llm_utils
import openai

from . import clangd_lsp_integration
from . import git_integration


class Functions:
    def __init__(self, args: argparse.Namespace):
        self.args = args

    def as_tools(self) -> list[openai.types.responses.FunctionToolParam]:
        tools: list[openai.types.responses.FunctionToolParam] = []
        fns = [
            self.get_compile_or_run_command,
            self.get_code_surrounding,
            self.list_directory,
        ]

        if clangd_lsp_integration.is_available():
            fns.append(self.find_definition)
        else:
            print(
                "[WARNING] clangd is not available. The 'find_definition' function will not be made available."
            )

        if git_integration.is_available():
            fns.append(self.git_status)
            fns.append(self.git_diff)
        else:
            print(
                "[WARNING] `git status` is not available. The associated git functions will not be made available."
            )

        for f in fns:
            assert f.__doc__
            tools.append(
                {
                    "type": "function",
                    "name": f.__name__,
                    **json.loads(f.__doc__),
                }
            )
        return tools

    def dispatch(
        self, function_call: openai.types.responses.ResponseFunctionToolCall
    ) -> Optional[str]:
        arguments = json.loads(function_call.arguments)
        print(
            f"Calling: {function_call.name}({', '.join([f'{k}={v}' for k, v in arguments.items()])})"
        )
        try:
            if function_call.name == "get_compile_or_run_command":
                return self.get_compile_or_run_command()
            elif function_call.name == "get_code_surrounding":
                return self.get_code_surrounding(
                    arguments["filename"], arguments["lineno"]
                )
            elif function_call.name == "list_directory":
                return self.list_directory(arguments["path"])
            elif function_call.name == "find_definition":
                return self.find_definition(
                    arguments["filename"], arguments["line_number"], arguments["symbol"]
                )
            elif function_call.name == "git_status":
                return self.git_status()
            elif function_call.name == "git_diff":
                return self.git_diff(arguments["filename"])
            else:
                print(f"[ERROR] Unknown function: {function_call.name}")
        except Exception as e:
            print(e)
        return None

    def get_compile_or_run_command(self) -> str:
        """
        {
            "description": "Returns the command used to compile or run the code. This will include any flags and options used."
        }
        """
        result = " ".join(self.args.command)
        print(result)
        return result

    def get_code_surrounding(self, filename: str, lineno: int) -> str:
        """
        {
            "description": "Returns the code in the given file surrounding and including the provided line number.",
            "strict": true,
            "parameters": {
                "type": "object",
                "properties": {
                    "filename": {
                        "type": "string",
                        "description": "The filename to read from."
                    },
                    "lineno": {
                        "type": "integer",
                        "description": "The line number to focus on. Some context before and after that line will be provided."
                    }
                },
                "required": ["filename", "lineno"],
                "additionalProperties": false
            }
        }
        """
        (lines, first) = llm_utils.read_lines(filename, lineno - 7, lineno + 3)
        result = llm_utils.number_group_of_lines(lines, first)
        print(result)
        return result

    def list_directory(self, path: str) -> str:
        """
        {
            "description": "Returns a list of all files and directories in the given directory.",
            "strict": true,
            "parameters": {
                "type": "object",
                "properties": {
                    "path": {
                        "type": "string",
                        "description": "The path of the directory to list."
                    }
                },
                "required": ["path"],
                "additionalProperties": false
            }
        }
        """
        entries = os.listdir(path)
        for i in range(len(entries)):
            if os.path.isdir(os.path.join(path, entries[i])):
                entries[i] += "/"
        result = "\n".join(entries)
        print(result)
        return result

    def find_definition(self, filename: str, line_number: int, symbol: str) -> str:
        """
        {
            "description": "Returns the source code for the definition for the given symbol at the given source line number.",
            "strict": true,
            "parameters": {
                "type": "object",
                "properties": {
                    "filename": {
                        "type": "string",
                        "description": "The filename the symbol is from."
                    },
                    "line_number": {
                        "type": "integer",
                        "description": "The line number where the symbol is present."
                    },
                    "symbol": {
                        "type": "string",
                        "description": "The symbol to lookup."
                    }
                },
                "required": ["filename", "line_number", "symbol"],
                "additionalProperties": false
            }
        }
        """
        result = clangd_lsp_integration.definition_plus_heuristics(
            filename, line_number, symbol
        )
        print(result)
        return result

    def git_status(self) -> str:
        """
        {
            "description": "Returns the git status (--porcelain) of the current repository.",
            "strict": true,
            "parameters": {
                "type": "object",
                "properties": {},
                "required": [],
                "additionalProperties": false
            }
        }
        """
        status = git_integration.get_git_status_porcelain()
        print(status)
        return status

    def git_diff(self, filename: str) -> str:
        """
        {
            "description": "Returns the diff since the last commit of the given file.",
            "strict": true,
            "parameters": {
                "type": "object",
                "properties": {
                    "filename": {
                        "type": "string",
                        "description": "The filename path to get the git diff for (relative to the current directory)."
                    }
                },
                "required": ["filename"],
                "additionalProperties": false
            }
        }
        """
        diff = git_integration.get_git_diff(filename)
        print(diff)
        return diff
