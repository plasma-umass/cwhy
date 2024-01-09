#! /usr/bin/env python3

import argparse
import importlib.metadata
import os
import sys
import tempfile
import textwrap

import openai

from . import cwhy


def wrapper(args):
    return textwrap.dedent(
        f"""
        #! {sys.executable}
        from cwhy import cwhy
        cwhy.wrapper({args})
        """
    ).lstrip()


def main():
    parser = argparse.ArgumentParser(
        prog="cwhy",
        description="CWhy explains and fixes compiler diagnostic errors.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        add_help=False,
    )

    parser.add_argument(
        "--help",
        "-h",
        action="help",
        default=argparse.SUPPRESS,
        help="Show this help message and exit.",
    )
    parser.add_argument(
        "--version",
        action="version",
        version=f"%(prog)s v{importlib.metadata.metadata('cwhy')['Version']}",
        default=argparse.SUPPRESS,
        help="Print the version of CWhy and exit.",
    )

    parser.add_argument(
        "--llm",
        type=str,
        default="default",
        help="The language model to use, e.g., 'gpt-3.5-turbo' or 'gpt-4'. The default mode tries gpt-4 and falls back to gpt-3.5-turbo.",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=60,
        help="The timeout for API calls in seconds.",
    )
    # The default maximum context length for `gpt-3.5-turbo` is 4096 tokens.
    # We keep 256 tokens for other parts of the prompt, and split the remainder in two
    # for the error message and code sections, resulting in 1920 tokens for each.
    parser.add_argument(
        "--max-error-tokens",
        type=int,
        default=1920,
        help="The maximum number of tokens from the error message to send in the prompt.",
    )
    parser.add_argument(
        "--max-code-tokens",
        type=int,
        default=1920,
        help="The maximum number of code locations tokens to send in the prompt.",
    )

    parser.add_argument(
        "--show-prompt",
        action="store_true",
        help="When enabled, only print prompt and exit (for debugging purposes).",
    )
    parser.add_argument(
        "--wrapper",
        action="store_true",
        help="Enable compiler wrapper behavior.",
    )
    parser.add_argument(
        "--wrapper-compiler",
        metavar="COMPILER",
        type=str,
        default="c++",
        help="The underlying compiler. Only enabled with --wrapper.",
    )

    subparsers = parser.add_subparsers(title="subcommands", dest="subcommand")

    subparsers.add_parser("explain", help="Explain the diagnostic. (default)")
    subparsers.add_parser("fix", help="Propose a fix for the diagnostic.")
    subparsers.add_parser("diff", help="[experimental] Propose a fix in diff format.")
    subparsers.add_parser(
        "converse", help="[experimental] A back-and-forth mode with ChatGPT."
    )

    parser.set_defaults(subcommand="explain")

    args = parser.parse_args()

    if args.wrapper:
        with tempfile.NamedTemporaryFile(mode="w", delete=False) as f:
            f.write(wrapper(args))
        # NamedTemporaryFiles are not executable by default.
        # Set its mode to 755 here with an octal literal.
        os.chmod(f.name, 0o755)
        print(f.name)
    else:
        stdin = sys.stdin.read()
        if not stdin:
            return
        try:
            print(cwhy.main(args, stdin))
        except (openai.NotFoundError, openai.RateLimitError, openai.APITimeoutError):
            # This type of exceptions should have been handled down the stack.
            pass


if __name__ == "__main__":
    main()
