import collections
import os
import re
import subprocess
import sys
import textwrap
from typing import Dict, List, Tuple

import openai

from llm_utils import llm_utils


def complete(args, user_prompt, **kwargs):
    if "show_prompt" in args and args["show_prompt"]:
        print("===================== Prompt =====================")
        print(user_prompt)
        print("==================================================")
        sys.exit(0)

    try:
        client = openai.OpenAI(timeout=args["timeout"])
    except openai.OpenAIError:
        print("You need an OpenAI key to use this tool.")
        print("You can get a key here: https://platform.openai.com/account/api-keys")
        print("Set the environment variable OPENAI_API_KEY to your key value.")
        sys.exit(1)

    try:
        completion = client.chat.completions.create(
            model=args["llm"],
            messages=[{"role": "user", "content": user_prompt}],
            **kwargs,
        )
        return completion
    # TODO: Maybe an exception needs to be caught here when the user does not have access to the model.
    except openai.APITimeoutError:
        print("The OpenAI API timed out.")
        print("You can increase the timeout with the --timeout option.")

    sys.exit(1)


def evaluate_diff(args, stdin):
    prompt = base_prompt(args, stdin) + "Help fix this issue by providing a diff."
    completion = complete(
        args,
        prompt,
        functions=[
            {
                "name": "fix_error",
                "description": "Returns all modifications needed for the provided code to compile.",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "diff": {
                            "type": "object",
                            "properties": {
                                "modifications": {
                                    "type": "array",
                                    "items": {
                                        "type": "object",
                                        "properties": {
                                            "filename": {"type": "string"},
                                            "start-line-number": {"type": "integer"},
                                            "number-lines-remove": {"type": "integer"},
                                            "replacement": {"type": "string"},
                                        },
                                        "required": [
                                            "filename",
                                            "start-line-number",
                                            "number-lines-remove",
                                            "replacement",
                                        ],
                                    },
                                },
                            },
                            "required": ["modifications"],
                        }
                    },
                    "required": ["diff"],
                },
            }
        ],
        function_call={"name": "fix_error"},
    )

    return completion


def evaluate_with_fallback(args, stdin):
    DEFAULT_FALLBACK_MODELS = ["gpt-4", "gpt-3.5-turbo"]
    for i, model in enumerate(DEFAULT_FALLBACK_MODELS):
        if i != 0:
            print(f"Falling back to {model}...")
        args["llm"] = model
        try:
            return evaluate(args, stdin)
        except openai.AuthenticationError as e:
            # TODO: Make sure this is still the right exception to catch for access issues.
            print(e)


def evaluate(args, stdin):
    if args["llm"] == "default":
        return evaluate_with_fallback(args, stdin)

    if args["subcommand"] == "explain":
        return evaluate_text_prompt(args, explain_prompt(args, stdin))
    elif args["subcommand"] == "fix":
        return evaluate_text_prompt(args, fix_prompt(args, stdin))
    elif args["subcommand"] == "diff":
        return evaluate_diff(args, stdin).choices[0].message.function_call.arguments
    else:
        raise Exception(f"unknown subcommand: {args['subcommand']}")


def evaluate_text_prompt(args, prompt, wrap=True, **kwargs):
    completion = complete(args, prompt, **kwargs)
    text = completion.choices[0].message.content

    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    cost = llm_utils.calculate_cost(
        completion.usage.prompt_tokens, completion.usage.completion_tokens, args["llm"]
    )
    text += "\n\n"
    text += f"(Total cost: approximately ${cost:.2f} USD.)"

    return text


# Define error patterns with associated information. The numbers
# correspond to the groups matching file name and line number.
error_patterns = [
    (
        "C#",
        re.compile(
            r"([a-zA-Z0-9./][^:\r\n]+)\((\d+),(\d+)\): error ([A-Za-z0-9]+): (.*)"
        ),
        1,
        2,
    ),
    ("C/C++/Rust", re.compile(r"([a-zA-Z0-9./][^:->]+):([0-9]+):([0-9]+)"), 1, 2),
    (
        "Visual Studio C/C++",
        re.compile(r"([a-zA-Z]?:?[\\\/a-zA-Z0-9._-]+)\(([0-9]+)\)"),
        1,
        2,
    ),
    # Note: LaTeX must precede Java
    ("LaTeX", re.compile(r"(.*\.tex):(\d+): error: (.*)"), 1, 2),
    ("Java", re.compile(r"([a-zA-Z0-9./][^:->]+):([0-9]+): error:"), 1, 2),
    ("Python", re.compile(r'\s*File "(.*?)", line (\d+), in ([^\<].*)'), 1, 2),
    ("Go", re.compile(r"([a-zA-Z0-9./][^:\r\n]+):([0-9]+):([0-9]+): (.*): (.*)"), 1, 2),
    (
        "TypeScript",
        re.compile(
            r"([a-zA-Z0-9./][^:\r\n]+)\((\d+),(\d+)\): error ([A-Za-z0-9]+): (.*)"
        ),
        1,
        2,
    ),
    ("Ruby", re.compile(r'"(.*\.rb)", line (\d+)(?:, in `.*\')?: (.*)'), 1, 2),
    ("PHP", re.compile(r"PHP (?:Parse|Fatal) error: (.*) in (.*) on line (\d+)"), 2, 3),
]


class explain_context:
    def __init__(self, args, diagnostic):
        self.args = args
        self.diagnostic_lines = diagnostic.splitlines()

        # We group by source file.
        self.code_locations = collections.defaultdict(dict)

        # Go through the diagnostic and build up a list of code locations.
        for linenum, line in enumerate(self.diagnostic_lines):
            file_name = None
            line_number = None
            for lang, pattern, file_group, line_group in error_patterns:
                match = pattern.match(line)
                # Rule out messages that contain the word 'warning' (for LaTeX; these match Java's regex)
                if match and "warning" not in line.lower():
                    # Extract information based on group indices
                    file_name = match.group(file_group).lstrip()
                    line_number = int(match.group(line_group))
                    break  # Move to the next line after a match

            if not file_name:
                continue

            try:
                (abridged_code, line_start) = llm_utils.read_lines(
                    file_name, line_number - 7, line_number + 3
                )
            except FileNotFoundError:
                print(
                    f"Cwhy warning: file not found: {file_name.lstrip()}",
                    file=sys.stderr,
                )
                continue

            for i, line_content in enumerate(abridged_code):
                self.code_locations[file_name][line_start + i] = line_content

    def get_diagnostic(self) -> str:
        """
        Alternate taking front and back lines until the maximum number of tokens.
        """
        front = []
        back = []
        n = len(self.diagnostic_lines)

        def build_diagnostic_string():
            return (
                "```\n"
                + "\n".join(front)
                + "\n\n[...]\n\n"
                + "\n".join(reversed(back))
                + "\n```\n"
            )

        for i in range(n):
            if i % 2 == 0:
                line = self.diagnostic_lines[i // 2]
                list = front
            else:
                line = self.diagnostic_lines[n - i // 2 - 1]
                list = back
            list.append(line)
            count = llm_utils.count_tokens(self.args["llm"], build_diagnostic_string())
            if count > self.args["max_error_tokens"]:
                list.pop()
                break
        return build_diagnostic_string()

    def get_code(self):
        if not self.code_locations:
            return None

        def format_group_code_block(group: List[str], last: int) -> str:
            """
            Format a group of consecutive lines from a single file as a code block.
            Include line numbers in front of each line.
            Trim first / last few lines if they are blank.

            Args:
                group: The list of lines.
                last: The line number of the last line in group.

            Returns:
                The formatted code block.
            """
            while group and not group[0].strip():
                group = group[1:]
            while group and not group[-1].strip():
                group = group[:-1]
                last -= 1

            first = last - len(group) + 1
            max_line_number_length = len(str(last))
            result = "```\n"
            for i, line in enumerate(group):
                result += "{0:>{1}} {2}\n".format(
                    first + i, max_line_number_length, line
                )
            result += "```\n\n"
            return result

        def format_file_locations(filename: str, lines: Dict[int, str]) -> str:
            """
            Format all the lines from a single file as a code block.
            There may be multiple groups: lines 1-10 and 100-110 for example.

            Args:
                filename: The name of the file lines originate from.
                lines: A mapping of line numbers to the corresponding line content.

            Returns:
                One or more concatenated formatted code blocks.
            """
            # Sort lines by line number.
            lines = sorted(lines.items(), key=lambda x: x[0])

            result = ""
            last = None
            group = []
            for line_number, line_content in lines:
                if last is None or line_number == last + 1:
                    group.append(line_content)
                    last = line_number
                else:
                    result += f"File `{filename}`:\n"
                    result += format_group_code_block(group, last)
                    last = None
                    group = []
            if last is not None:
                result += f"File `{filename}`:\n"
                result += format_group_code_block(group, last)
            return result

        formatted_file_locations = [
            format_file_locations(filename, lines)
            for filename, lines in self.code_locations.items()
        ]

        counts = [
            llm_utils.count_tokens(self.args["llm"], x)
            for x in formatted_file_locations
        ]
        index = 0
        total = 0
        while (
            index < len(counts)
            and total + counts[index] <= self.args["max_code_tokens"]
        ):
            total += counts[index]
            index += 1
        return "".join(formatted_file_locations[:index])


def base_prompt(args, diagnostic):
    ctx = explain_context(args, diagnostic)

    user_prompt = ""
    code = ctx.get_code()
    if code:
        user_prompt += "This is my code:\n\n"
        user_prompt += code
        user_prompt += "\n"
    user_prompt += "This is my error:\n"
    user_prompt += ctx.get_diagnostic()
    user_prompt += "\n\n"

    return user_prompt


def explain_prompt(args, diagnostic):
    return base_prompt(args, diagnostic) + "What's the problem?"


def fix_prompt(args, diagnostic):
    return (
        base_prompt(args, diagnostic)
        + "Suggest code to fix the problem. Surround the code in backticks (```)."
    )


def wrapper(args):
    process = subprocess.run(
        [args["wrapper_compiler"], *sys.argv[1:]],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )

    status = process.returncode
    if status != 0:
        print(process.stdout)
        if "CWHY_DISABLE" not in os.environ:
            print("==================================================")
            print("CWhy")
            print("==================================================")
            print(evaluate(args, process.stdout))
            print("==================================================")
    sys.exit(status)
