#! /usr/bin/env python3
import asyncio
import io
import openai
import openai_async
import re
import sys
import textwrap

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
    paragraphs = text.split('\n\n')
    wrapped_paragraphs = []
    # Check if currently in a code block.
    in_code_block = False
    # Loop through each paragraph and apply appropriate wrapping.
    for paragraph in paragraphs:
        # If this paragraph starts and ends with a code block, add it as is.
        if paragraph.startswith('```') and paragraph.endswith('```'):
            wrapped_paragraphs.append(paragraph)
            continue
        # If this is the beginning of a code block add it as is.
        if paragraph.startswith('```'):
            in_code_block = True
            wrapped_paragraphs.append(paragraph)
            continue
        # If this is the end of a code block stop skipping text.
        if paragraph.endswith('```'):
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
    wrapped_text = '\n\n'.join(wrapped_paragraphs)
    return wrapped_text

def read_lines(file_path: str, start_line: int, end_line: int) -> (str, int):
    """
    Read lines from a file.

    Args:
        file_path (str): The path of the file to read.
        start_line (int): The line number of the first line to include (1-indexed).
        end_line (int): The line number of the last line to include.

    Returns:
        (str, end_line): The content read and the last line included.

    """
    # open the file for reading
    with open(file_path, 'r') as f:
        # read all the lines from the file
        lines = f.readlines()
        # remove trailing newline characters
        lines = [line.rstrip() for line in lines]
    # convert start_line to 0-based indexing and ensure it's in range
    start_line = max(0, start_line - 1)
    # ensure end_line is within range
    end_line = min(len(lines), end_line)
    # return the requested lines as a list
    return ('\n'.join(lines[start_line:end_line]) + '\n', start_line, end_line)

class context(object):
    def __init__(self, diagnostic):
        diagnostic_lines = diagnostic.readlines()
        diagnostic_lines = [line.decode() for line in diagnostic_lines]

        self.code_locations = {}

        # Magic number - don't send more than this many code locations.
        # This is just to prevent overwhelming OpenAI.
        max_code_locations = 10

        # Go through the compiler output and build up a list of code locations.
        line = 0
        while line < len(diagnostic_lines):
            match = re.search(r'^([^:->]+):([0-9]+):([0-9]+)', diagnostic_lines[line])

            line += 1

            if not match:
                continue

            if max_code_locations == 0:
                # We've found the end of the last "frame", and we don't have room
                # for anymore, so we're done.
                break

            file_name = match.group(1)
            line_number = int(match.group(2))

            (abridged_code, line_start, line_end) = read_lines(file_name, line_number - 7, line_number + 2)

            # Avoid duplicates.
            if (file_name, line_start, line_end) not in self.code_locations:
              self.code_locations[(file_name, line_start, line_end)] = abridged_code
              max_code_locations -= 1

        self.unabridged_diagnostic = '\n'.join(diagnostic_lines) + '\n'
        self.abridged_diagnostic = '\n'.join(diagnostic_lines[0:line]) + '\n'

        def format_code_location(code_location):
            ((file_name, line_start, line_end), abridged_code) = code_location
            s = 'File `{}` line {} to {}:\n'.format(file_name, line_start, line_end)
            s += '\n'
            s += '```\n'
            s += abridged_code
            s += '```\n'
            return s

        self.code = '\n'.join([format_code_location(cl) for cl in self.code_locations.items()]) + '\n'

async def complete(user_prompt):
    try:
        completion = await openai_async.chat_complete(openai.api_key, timeout=30, payload={'model': 'gpt-3.5-turbo', 'messages': [{'role': 'user', 'content': user_prompt}]})
        json_payload = completion.json()
        text = json_payload['choices'][0]['message']['content']
    except (openai.error.AuthenticationError, httpx.LocalProtocolError, KeyError):
        # Something went wrong.
        print()
        print('You need a valid OpenAI key to use ChatDBG. You can get a key here: https://openai.com/api/')
        print('Set the environment variable OPENAI_API_KEY to your key value.')
        import sys
        sys.exit(1)
    except Exception as e:
        print(f'EXCEPTION {e}, {type(e)}')
        pass
    return text

def cwhy_prompt(fix):
    with io.open(sys.stdin.fileno(), "rb", closefd=False) as stdin:
        ctx = context(stdin)

        if fix:
            user_prompt = f"""
            This is my code:

            {ctx.code}

            This is my error:

            {ctx.abridged_diagnostic}

            Suggest code to fix the problem. Surround the code in backticks (```).
            """
        else:
            user_prompt = f"""
            This is my code:

            {ctx.code}

            This is my error:

            {ctx.abridged_diagnostic}

            What's the problem?
            """

        return user_prompt

