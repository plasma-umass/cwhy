import argparse
import difflib
import json
import subprocess
import sys
import traceback
from typing import Optional

from . import utils
from .explain_functions import ExplainFunctions


class DiffFunctions:
    def __init__(self, args: argparse.Namespace):
        self.args = args
        self.explain_functions = ExplainFunctions(args)

    def as_tools(self):
        return self.explain_functions.as_tools() + [
            {"type": "function", "function": schema}
            for schema in [
                self.apply_modification_schema(),
                self.try_compiling_schema(),
            ]
        ]

    def dispatch(self, function_call) -> Optional[str]:
        arguments = json.loads(function_call.arguments)
        try:
            if function_call.name == "apply_modification":
                print("Calling: apply_modification(...)")
                return self.apply_modification(
                    arguments["filename"],
                    arguments["start-line-number"],
                    arguments["number-lines-remove"],
                    arguments["replacement"],
                )
            elif function_call.name == "try_compiling":
                print("Calling: try_compiling()")
                return self.try_compiling()
            else:
                return self.explain_functions.dispatch(function_call)
        except Exception:
            traceback.print_exc()
            return None

    def apply_modification_schema(self):
        return {
            "name": "apply_modification",
            "description": "Applies a single modification to the source file with the goal of fixing any existing compilation errors.",
            "parameters": {
                "type": "object",
                "properties": {
                    "filename": {
                        "type": "string",
                        "description": "The filename to modify.",
                    },
                    "start-line-number": {
                        "type": "integer",
                        "description": "The line number to start replacing at.",
                    },
                    "number-lines-remove": {
                        "type": "integer",
                        "description": "The number of lines to remove, which can be zero to only add new code.",
                    },
                    "replacement": {
                        "type": "string",
                        "description": "The replacement code, which can be blank to simply remove lines.",
                    },
                },
                "required": [
                    "filename",
                    "start-line-number",
                    "number-lines-remove",
                    "replacement",
                ],
            },
        }

    def apply_modification(
        self,
        filename: str,
        start_line_number: int,
        number_lines_remove: int,
        replacement: str,
    ) -> Optional[str]:
        with open(filename, "r") as f:
            lines = [line.rstrip() for line in f.readlines()]

        pre_lines = lines[: start_line_number - 1]
        replacement_lines = replacement.splitlines()
        replaced_lines = lines[
            start_line_number - 1 : start_line_number + number_lines_remove - 1
        ]
        post_lines = lines[start_line_number + number_lines_remove - 1 :]

        # If replacing a single line, make sure we keep indentation.
        if (
            number_lines_remove == 1
            and len(replacement_lines) == 1
            and start_line_number >= 1
        ):
            replaced_line = lines[start_line_number - 1]
            replacement_lines[0] = replacement_lines[0].lstrip()
            n = len(replaced_line) - len(replaced_line.lstrip())
            whitespace = replaced_line[:n]
            replacement_lines[0] = whitespace + replacement_lines[0]

        print("CWhy wants to do the following modification:")
        for line in difflib.unified_diff(replaced_lines, replacement_lines):
            print(line)
        if not input("Is this modification okay? (y/n) ") == "y":
            return "The user declined this modification, it is probably wrong."

        lines = pre_lines + replacement_lines + post_lines
        with open(filename, "w") as f:
            f.write("\n".join(lines))
        return "Modification applied."

    def try_compiling_schema(self):
        return {
            "name": "try_compiling",
            "description": "Attempts to compile the code again after the user has made changes. Returns the new error message if there is one.",
        }

    def try_compiling(self) -> Optional[str]:
        process = subprocess.run(
            self.args.command,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )

        if process.returncode == 0:
            print("Compilation successful!")
            sys.exit(0)

        return utils.get_truncated_error_message(self.args, process.stderr)
