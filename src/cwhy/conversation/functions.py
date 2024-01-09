import json

from llm_utils import llm_utils


class Functions:
    def __init__(self, args, diagnostic):
        self.args = args
        self.diagnostic = diagnostic

    def as_tools(self):
        return [
            {"type": "function", "function": schema}
            for schema in [
                # self.get_truncated_error_message_schema(),
                # self.get_compile_or_run_command_schema(),
                self.get_code_surrounding_schema(),
            ]
        ]

    def dispatch(self, function_call) -> str:
        arguments = json.loads(function_call.arguments)
        print(
            f"Calling: {function_call.name}({', '.join([f'{k}={v}' for k, v in arguments.items()])})"
        )
        try:
            if function_call.name == "get_truncated_error_message":
                return self.get_truncated_error_message()
            elif function_call.name == "get_compile_or_run_command":
                return self.get_compile_or_run_command()
            elif function_call.name == "get_code_surrounding":
                return self.get_code_surrounding(
                    arguments["filename"], arguments["lineno"]
                )
        except Exception as e:
            print(e)
            pass
        return None

    def get_truncated_error_message_schema(self):
        return {
            "name": "get_truncated_error_message",
            "description": f"Returns the original error message, truncating to {self.args.max_error_tokens} tokens by keeping the beginning and end of the message.",
        }

    def get_truncated_error_message(self) -> str:
        """
        Alternate taking front and back lines until the maximum number of tokens.
        """
        front = []
        back = []
        diagnostic_lines = self.diagnostic.splitlines()
        n = len(diagnostic_lines)

        def build_diagnostic_string():
            return "\n".join(front) + "\n\n[...]\n\n" + "\n".join(reversed(back)) + "\n"

        for i in range(n):
            if i % 2 == 0:
                line = diagnostic_lines[i // 2]
                list = front
            else:
                line = diagnostic_lines[n - i // 2 - 1]
                list = back
            list.append(line)
            count = llm_utils.count_tokens(self.args.llm, build_diagnostic_string())
            if count > self.args.max_error_tokens:
                list.pop()
                break
        return build_diagnostic_string()

    def get_compile_or_run_command_schema():
        return {
            "name": "get_compile_or_run_command",
            "description": "Returns the command used to compile or run the code.",
        }

    def get_compile_or_run_command() -> str:
        # TODO.
        pass

    def get_code_surrounding_schema(self):
        return {
            "name": "get_code_surrounding",
            "description": "Returns the code in the given file surrounding and including the provided line number.",
            "parameters": {
                "type": "object",
                "properties": {
                    "filename": {
                        "type": "string",
                        "description": "The filename to read from.",
                    },
                    "lineno": {
                        "type": "integer",
                        "description": "The line number to focus on. Some context before and after that line will be provided.",
                    },
                },
                "required": ["filename", "lineno"],
            },
        }

    def get_code_surrounding(self, filename: str, lineno: int) -> str:
        def format_group_code_block(group: list[str], first: int) -> str:
            while group and not group[0].strip():
                group = group[1:]
                first += 1
            while group and not group[-1].strip():
                group = group[:-1]

            last = first + len(group) - 1
            max_line_number_length = len(str(last))
            result = "\n".join(
                [
                    "{0:>{1}} {2}".format(first + i, max_line_number_length, line)
                    for i, line in enumerate(group)
                ]
            )
            return result

        (lines, first) = llm_utils.read_lines(filename, lineno - 7, lineno + 3)
        return format_group_code_block(lines, first)