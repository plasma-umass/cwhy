#! /usr/bin/env python3

import argparse
import importlib.metadata
import sys

from . import cwhy


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

    subparsers = parser.add_subparsers(title="subcommands", dest="subcommand")
    subparsers.add_parser(
        "explain", help="explain the diagnostic. This is the default subcommand"
    ).set_defaults(fn=explain)
    subparsers.add_parser("fix", help="propose a fix for the diagnostic").set_defaults(
        fn=fix
    )
    subparsers.add_parser(
        "extract_sources",
        help="extract the source locations from the diagnostic as CSV",
    ).set_defaults(fn=extract_sources)

    args = parser.parse_args()

    if args.version:
        print(f"cwhy version {importlib.metadata.metadata('cwhy')['Version']}")
        return

    if args.subcommand is None:
        args.fn = explain

    args.fn(args)


def explain(args):
    cwhy.evaluate_prompt(args, cwhy.explain_prompt(sys.stdin.read()))


def fix(args):
    cwhy.evaluate_prompt(args, cwhy.fix_prompt(sys.stdin.read()))


def extract_sources(args):
    cwhy.evaluate_prompt(
        args,
        cwhy.extract_sources_prompt(sys.stdin.read()),
        wrap=False,
    )
