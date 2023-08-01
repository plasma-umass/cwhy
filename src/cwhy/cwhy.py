import re
import sys
import textwrap

import openai


def word_wrap_except_code_blocks(text: str) -> str:
    """Wraps text except for code blocks.

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
        (str, end_line): The content read and the last line included.

    Raises:
        FileNotFoundError: If the file does not exist.
    """
    with open(file_path, "r") as f:
        lines = f.readlines()
        lines = [line.rstrip() for line in lines]

    # Ensure indices are in range.
    start_line = max(1, start_line)
    end_line = min(len(lines), end_line)

    return ("\n".join(lines[start_line - 1 : end_line]) + "\n", start_line, end_line)


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


class explain_context:
    def __init__(self, args, diagnostic):
        diagnostic_lines = diagnostic.splitlines()

        self.code_locations = {}

        # Don't send more than this many code locations.
        # This is just to prevent overwhelming OpenAI.
        max_code_locations = args["max_context"]

        # Go through the diagnostic and build up a list of code locations.
        line = 0
        while line < len(diagnostic_lines):
            # This pattern works for some C++ compilers (GCC, Clang) and Rust.
            match = re.match(
                r"([a-zA-Z0-9./][^:->]+):([0-9]+):([0-9]+)", diagnostic_lines[line]
            )

            if not match:
                # This pattern works for javac.
                match = re.match(
                    r"([a-zA-Z0-9./][^:->]+):([0-9]+):", diagnostic_lines[line]
                )

            if not match:
                # This pattern works for Python, filtering out non-files (e.g., <string>).
                match = re.match(
                    r'\s*File "(.*?)", line (\d+), in ([^\<].*)', diagnostic_lines[line]
                )

            line += 1

            if not match:
                continue

            if max_code_locations == 0:
                # We've found the end of the last "frame", and we don't have room
                # for anymore, so we're done.
                break

            file_name = match.group(1).lstrip()
            line_number = int(match.group(2))

            try:
                (abridged_code, line_start, line_end) = read_lines(
                    file_name, line_number - 7, line_number + 2
                )
            except FileNotFoundError:
                print(f"Cwhy warning: file not found: {file_name.lstrip()}")
                continue

            # Avoid duplicates.
            if (file_name, line_start, line_end) not in self.code_locations:
                self.code_locations[(file_name, line_start, line_end)] = abridged_code
                max_code_locations -= 1

        # If the diagnostic didn't come from a context that we know about and
        # handle in the above loop, we should make sure it's not too long.
        if not self.code_locations and line == len(diagnostic_lines) - 1:
            line = min(line, 50)

        self.unabridged_diagnostic = "\n".join(diagnostic_lines) + "\n"
        self.abridged_diagnostic = (
            "```\n" + "\n".join(diagnostic_lines[:line]) + "\n```\n"
        )

        def format_code_location(code_location):
            ((file_name, line_start, line_end), abridged_code) = code_location
            s = "File `{}` line {} to {}:\n".format(file_name, line_start, line_end)
            s += "\n"
            s += "```\n"
            s += abridged_code
            s += "```\n"
            return s

        self.code = (
            "\n".join([format_code_location(cl) for cl in self.code_locations.items()])
            + "\n"
        )


def base_prompt(args, diagnostic):
    ctx = explain_context(args, diagnostic)

    user_prompt = ""
    if ctx.code:
        user_prompt += "This is my code:\n\n"
        user_prompt += ctx.code
        user_prompt += "\n"
    user_prompt += "This is my error:\n\n"
    user_prompt += ctx.abridged_diagnostic
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
        self.unabridged_diagnostic = "\n".join(diagnostic_lines) + "\n"
        self.abridged_diagnostic = (
            "```\n" + "\n".join(diagnostic_lines[:line]) + "\n```\n"
        )


def extract_sources_prompt(diagnostic):
    ctx = extract_sources_context(diagnostic)
    user_prompt = "Respond only in the CSV format with no header row.\n"
    user_prompt += "Identify all of the file paths and associated line numbers.\n"
    user_prompt += "Output each file path and associated line number.\n"
    user_prompt += "\n"
    user_prompt += ctx.abridged_diagnostic

    return user_prompt
