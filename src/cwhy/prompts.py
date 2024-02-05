import argparse
import collections
import re
import sys
from typing import Dict, List, Optional

import llm_utils


# Define error patterns with associated information. The numbers
# correspond to the groups matching file name and line number.
_error_patterns = [
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


class _Context:
    def __init__(self, args: argparse.Namespace, diagnostic: str):
        self.args = args
        self.diagnostic_lines = diagnostic.splitlines()

        # We group by source file.
        self.code_locations: Dict[str, Dict[int, str]] = collections.defaultdict(dict)

        # Go through the diagnostic and build up a list of code locations.
        for line in self.diagnostic_lines:
            file_name = None
            line_number = None
            for _, pattern, file_group, line_group in _error_patterns:
                match = pattern.match(line)
                # Rule out messages that contain the word 'warning' (for LaTeX; these match Java's regex)
                if match and "warning" not in line.lower():
                    # Extract information based on group indices
                    file_name = match.group(file_group).lstrip()
                    line_number = int(match.group(line_group))
                    break  # Move to the next line after a match

            if not file_name or not line_number:
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
        front: List[str] = []
        back: List[str] = []
        n = len(self.diagnostic_lines)

        def build_diagnostic_string() -> str:
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
            count = llm_utils.count_tokens(self.args.llm, build_diagnostic_string())
            if count > self.args.max_error_tokens:
                list.pop()
                break
        return build_diagnostic_string()

    def get_code(self) -> Optional[str]:
        if not self.code_locations:
            return None

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
            sorted_lines = sorted(lines.items(), key=lambda x: x[0])

            result = ""
            last = None
            group = []
            for line_number, line_content in sorted_lines:
                if last is None or line_number == last + 1:
                    group.append(line_content)
                    last = line_number
                else:
                    result += f"File `{filename}`:\n```\n"
                    result += llm_utils.number_group_of_lines(
                        group, last - len(group) + 1
                    )
                    result += "\n```\n\n"
                    last = None
                    group = []
            if last is not None:
                result += f"File `{filename}`:\n```\n"
                result += llm_utils.number_group_of_lines(group, last - len(group) + 1)
                result += "\n```\n\n"
            return result

        formatted_file_locations = [
            format_file_locations(filename, sorted_lines)
            for filename, sorted_lines in self.code_locations.items()
        ]

        counts = [
            llm_utils.count_tokens(self.args.llm, x) for x in formatted_file_locations
        ]
        index = 0
        total = 0
        while (
            index < len(counts) and total + counts[index] <= self.args.max_code_tokens
        ):
            total += counts[index]
            index += 1
        return "".join(formatted_file_locations[:index])


def _base_prompt(args: argparse.Namespace, diagnostic: str) -> str:
    ctx = _Context(args, diagnostic)

    prompt = ""
    code = ctx.get_code()
    if code:
        prompt += "This is my code:\n\n"
        prompt += code
        prompt += "\n"
    prompt += "This is my error:\n"
    prompt += ctx.get_diagnostic()
    prompt += "\n\n"

    return prompt


def explain_prompt(args: argparse.Namespace, diagnostic: str) -> str:
    return (
        _base_prompt(args, diagnostic)
        + "What's the problem? If you can, suggest code to fix the issue."
    )


def diff_prompt(args: argparse.Namespace, diagnostic: str) -> str:
    return (
        _base_prompt(args, diagnostic)
        + "Help fix this issue by providing a diff in JSON format."
    )
