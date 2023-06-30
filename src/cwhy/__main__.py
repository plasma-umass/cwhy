#! /usr/bin/env python3

import argparse
import importlib.metadata
import os
import sys
import tempfile

from . import cwhy


def wrapper(args):
    with open(os.path.join(os.path.dirname(__file__), "wrapper.py.in")) as f:
        template = f.read()
    return template.format(compiler=args["compiler"], args=args)


def main():
    parser = argparse.ArgumentParser(
        prog="cwhy",
        description="CWhy provides utilities to explain diagnostic messages using LLMs.",
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
        help="the language model to use. this tool is tested with 'gpt-3.5-turbo' and 'gpt-4'",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=60,
        help="timeout for API calls in seconds",
    )

    parser.add_argument(
        "--show-prompt",
        action="store_true",
        help="print prompts before sending them to OpenAI for debugging",
    )
    parser.add_argument(
        "--wrapper",
        action="store_true",
        help="enable compiler wrapper behavior",
    )

    subparsers = parser.add_subparsers(title="subcommands", dest="subcommand")

    subparsers.add_parser("explain", help="explain the diagnostic (default)")
    subparsers.add_parser("fix", help="propose a fix for the diagnostic")
    subparsers.add_parser(
        "extract-sources", help="extract the source locations from the diagnostic"
    )
    subparsers.add_parser(
        "wrapper", help="behave like a compiler wrapper"
    ).add_argument(
        "--compiler", type=str, default="c++", help="the underlying compiler"
    )

    parser.set_defaults(subcommand="explain")

    args = vars(parser.parse_args())

    if args["subcommand"] == "wrapper":
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
            if args["subcommand"] == "explain":
                cwhy.evaluate_prompt(args, cwhy.explain_prompt(stdin))
            elif args["subcommand"] == "fix":
                cwhy.evaluate_prompt(args, cwhy.fix_prompt(stdin))
            elif args["subcommand"] == "extract-sources":
                cwhy.evaluate_prompt(
                    args, cwhy.extract_sources_prompt(stdin), wrap=False
                )
            else:
                raise Exception("unreachable")
