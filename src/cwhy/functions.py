import argparse
import json
import os
from typing import Optional

import llm_utils
import openai


class Functions:
    def __init__(self, args: argparse.Namespace):
        self.args = args

    def as_tools(self) -> list[openai.types.responses.FunctionToolParam]:
        tools: list[openai.types.responses.FunctionToolParam] = []
        for f in [
            self.get_compile_or_run_command,
            self.get_code_surrounding,
            self.list_directory,
        ]:
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
        return "\n".join(entries)
