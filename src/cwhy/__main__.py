#! /usr/bin/env python3

import argparse
import importlib.metadata
import os
import sys
import tempfile
import textwrap

from . import cwhy


def wrapper(args):
    return textwrap.dedent(
        f"""
        #! /usr/bin/env python3
        from cwhy import cwhy
        cwhy.wrapper({args})
        """
    ).lstrip()


def main():
    parser = argparse.ArgumentParser(
        prog="cwhy",
        description="CWhy explains and fixes compiler diagnostic errors.",
    )

    parser.add_argument(
        "--version",
        action="store_true",
        help="print the version of cwhy and exit",
    )

    parser.add_argument(
        "--llm",
        type=str,
        default="gpt-3.5-turbo",
        help="the language model to use, e.g., 'gpt-3.5-turbo' or 'gpt-4' (default: gpt-3.5-turbo)",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=60,
        help="timeout for API calls in seconds (default: 60)",
    )
    # The default maximum context length for `gpt-3.5-turbo` is 4096 tokens.
    # We keep 256 tokens for other parts of the prompt, and split the remainder in two
    # for the error message and code sections, resulting in 1920 tokens for each.
    parser.add_argument(
        "--max-error-tokens",
        type=int,
        default=1920,
        help="maximum number of tokens from the error message to send in the prompt (default: 1920)",
    )
    parser.add_argument(
        "--max-code-tokens",
        type=int,
        default=1920,
        help="maximum number of code locations tokens to send in the prompt (default: 1920)",
    )

    parser.add_argument(
        "--show-prompt",
        action="store_true",
        help="only print prompt and exit (for debugging purposes)",
    )
    parser.add_argument(
        "--wrapper",
        action="store_true",
        help="enable compiler wrapper behavior",
    )
    parser.add_argument(
        "--wrapper-compiler",
        metavar="COMPILER",
        type=str,
        default="c++",
        help="the underlying compiler. Only enabled with --wrapper",
    )

    subparsers = parser.add_subparsers(title="subcommands", dest="subcommand")

    subparsers.add_parser("explain", help="explain the diagnostic (default)")
    subparsers.add_parser("fix", help="propose a fix for the diagnostic")
    subparsers.add_parser("diff", help="propose a fix in diff format")
    subparsers.add_parser(
        "extract-sources", help="extract the source locations from the diagnostic"
    )

    parser.set_defaults(subcommand="explain")

    args = vars(parser.parse_args())

    if args["wrapper"]:
        with tempfile.NamedTemporaryFile(mode="w", delete=False) as f:
            f.write(wrapper(args))
        # NamedTemporaryFiles are not executable by default. Set its mode to 755 here with an octal literal.
        os.chmod(f.name, 0o755)
        print(f.name)
    elif args["version"]:
        print(f"cwhy version {importlib.metadata.metadata('cwhy')['Version']}")
    else:
        stdin = sys.stdin.read()
        if stdin:
            print(cwhy.evaluate(args, stdin))
