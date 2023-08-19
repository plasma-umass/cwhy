import re
import sys
import textwrap
import os
import subprocess
import collections
from typing import List, Tuple

import openai
import tiktoken


def word_wrap_except_code_blocks(text: str) -> str:
    """
    Wraps text except for code blocks.

    Splits the text into paragraphs and wraps each paragraph,
    except for paragraphs that are inside of code blocks denoted
    by ` ``` `. Returns the updated text.

    Args:
        text: The text to wrap.

    Returns:
        The wrapped text.
    """
    # Split text into paragraphs
    paragraphs = text.split("\n\n")
    wrapped_paragraphs = []
    # Check if currently in a code block.
    in_code_block = False
    # Loop through each paragraph and apply appropriate wrapping.
    for paragraph in paragraphs:
        # If this paragraph starts and ends with a code block, add it as is.
        if paragraph.startswith("```") and paragraph.endswith("```"):
            wrapped_paragraphs.append(paragraph)
            continue
        # If this is the beginning of a code block add it as is.
        if paragraph.startswith("```"):
            in_code_block = True
            wrapped_paragraphs.append(paragraph)
            continue
        # If this is the end of a code block stop skipping text.
        if paragraph.endswith("```"):
            in_code_block = False
            wrapped_paragraphs.append(paragraph)
            continue
        # If we are currently in a code block add the paragraph as is.
        if in_code_block:
            wrapped_paragraphs.append(paragraph)
        else:
            # Otherwise, apply text wrapping to the paragraph.
            wrapped_paragraph = textwrap.fill(paragraph)
            wrapped_paragraphs.append(wrapped_paragraph)
    # Join all paragraphs into a single string
    wrapped_text = "\n\n".join(wrapped_paragraphs)
    return wrapped_text


def read_lines(file_path, start_line, end_line):
    """
    Read lines from a file.

    Args:
        file_path (str): The path of the file to read.
        start_line (int): The line number of the first line to include (1-indexed). Will be bounded below by 0.
        end_line (int): The line number of the last line to include (1-indexed). Will be bounded above by file's line count.

    Returns:
        The lines read as an array and the number of the first line included.

    Raises:
        FileNotFoundError: If the file does not exist.
    """
    max_chars_per_line = 128  # Prevent pathological case where lines are REALLY long.

    def truncate(s, l):
        """
        Truncate the string to at most the given length, adding ellipses if truncated.
        """
        if len(s) < l:
            return s
        else:
            return s[:l] + "..."

    with open(file_path, "r") as f:
        lines = f.readlines()
        lines = [truncate(line.rstrip(), max_chars_per_line) for line in lines]

    # Ensure indices are in range.
    start_line = max(1, start_line)
    end_line = min(len(lines), end_line)

    return (lines[start_line - 1 : end_line], start_line)


def complete(args, user_prompt, **kwargs):
    try:
        if "show_prompt" in args and args["show_prompt"]:
            print("===================== Prompt =====================")
            print(user_prompt)
            print("==================================================")
            sys.exit(0)

        completion = openai.ChatCompletion.create(
            model=args["llm"],
            request_timeout=args["timeout"],
            messages=[{"role": "user", "content": user_prompt}],
            **kwargs,
        )
        return completion
    except openai.error.AuthenticationError:
        print("You need an OpenAI key to use this tool.")
        print("You can get a key here: https://platform.openai.com/account/api-keys")
        print("Set the environment variable OPENAI_API_KEY to your key value.")
        print(
            "If OPENAI_API_KEY is already correctly set, you may have exceeded your usage or rate limit."
        )
    except openai.error.Timeout:
        print(
            "The OpenAI API timed out. You can try increasing the timeout with the --timeout option."
        )

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


def evaluate(args, stdin):
    if args["subcommand"] == "explain":
        return evaluate_text_prompt(args, explain_prompt(args, stdin))
    elif args["subcommand"] == "fix":
        return evaluate_text_prompt(args, fix_prompt(args, stdin))
    elif args["subcommand"] == "diff":
        return evaluate_diff(args, stdin).choices[0].message.function_call.arguments
    elif args["subcommand"] == "extract-sources":
        return evaluate_text_prompt(args, extract_sources_prompt(stdin), wrap=False)
    else:
        raise Exception(f"unknown subcommand: {args['subcommand']}")


def evaluate_text_prompt(args, prompt, wrap=True, **kwargs):
    completion = complete(args, prompt, **kwargs)
    text = completion.choices[0].message.content
    if wrap:
        text = word_wrap_except_code_blocks(text)
    return text


# Define error patterns with associated information. The numbers
# correspond to the groups matching file name and line number.
error_patterns = [
    # C# error message pattern
    ("C#", re.compile(
        r"([a-zA-Z0-9./][^:\r\n]+)\((\d+),(\d+)\): error ([A-Za-z0-9]+): (.*)"
    ), 1, 2),
    # C/C++/Rust error message pattern
    ("C/C++/Rust", re.compile(
        r"([a-zA-Z0-9./][^:->]+):([0-9]+):([0-9]+)"
    ), 1, 2),
    # Java error message pattern
    ("Java", re.compile(
        r"([a-zA-Z0-9./][^:->]+):([0-9]+):"
    ), 1, 2),
    # Python error message pattern
    ("Python", re.compile(
        r'\s*File "(.*?)", line (\d+), in ([^\<].*)'
    ), 1, 2),
    # Go error message pattern
    ("Go", re.compile(
        r"([a-zA-Z0-9./][^:\r\n]+):([0-9]+):([0-9]+): (.*): (.*)"
    ), 1, 2),
    # TypeScript error message pattern
    ("TypeScript", re.compile(
        r"([a-zA-Z0-9./][^:\r\n]+)\((\d+),(\d+)\): error ([A-Za-z0-9]+): (.*)"
    ), 1, 2),
    # Ruby error message pattern
    ("Ruby", re.compile(
        r'"(.*\.rb)", line (\d+)(?:, in `.*\')?: (.*)'
    ), 1, 2),
    # PHP error message pattern
    ("PHP", re.compile(
        r"PHP (?:Parse|Fatal) error: (.*) in (.*) on line (\d+)"
    ), 2, 3),
]
class explain_context:
    def __init__(self, args, diagnostic):
        self.args = args
        self.encoding = tiktoken.encoding_for_model(args["llm"])
        self.diagnostic_lines = diagnostic.splitlines()

        # We group by source file, then keep line numbers ordered.
        self.code_locations = collections.defaultdict(collections.OrderedDict)

        # Go through the diagnostic and build up a list of code locations.
        for (linenum, line) in enumerate(self.diagnostic_lines):
            file_name = None
            line_number = None
            for lang, pattern, file_group, line_group in error_patterns:
                match = pattern.match(line)
                if match:
                    # Extract information based on group indices
                    file_name = match.group(file_group).lstrip()
                    line_number = int(match.group(line_group))
                    break  # Move to the next line after a match

            if not file_name:
                continue
            
            try:
                (abridged_code, line_start) = read_lines(
                    file_name, line_number - 7, line_number + 3
                )
            except FileNotFoundError:
                print(f"Cwhy warning: file not found: {file_name.lstrip()}")
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
            count = len(self.encoding.encode(build_diagnostic_string()))
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

        def format_file_locations(filename: str, lines: List[Tuple[int, str]]) -> str:
            """
            Format all the lines from a single file as a code block.
            There may be multiple groups: lines 1-10 and 100-110 for example.

            Args:
                filename: The name of the file lines originate from.
                lines: A list of line number to actual line content tuples.

            Returns:
                One or more concatenated formatted code blocks.
            """
            result = ""
            last = None
            group = []
            for line_number, line_content in lines.items():
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

        counts = [len(self.encoding.encode(x)) for x in formatted_file_locations]
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


class extract_sources_context:
    def __init__(self, diagnostic):
        diagnostic_lines = diagnostic.splitlines()
        line = min(len(diagnostic_lines) - 1, 50)
        self.diagnostic = "```\n" + "\n".join(diagnostic_lines[:line]) + "\n```\n"


def extract_sources_prompt(diagnostic):
    ctx = extract_sources_context(diagnostic)
    user_prompt = "Respond only in the CSV format with no header row.\n"
    user_prompt += "Identify all of the file paths and associated line numbers.\n"
    user_prompt += "Output each file path and associated line number.\n"
    user_prompt += "\n"
    user_prompt += ctx.diagnostic

    return user_prompt


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
