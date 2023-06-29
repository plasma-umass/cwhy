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

    args = parser.parse_args()

    if args.version:
        print(f"cwhy version {importlib.metadata.metadata('cwhy')['Version']}")
        return

    cwhy.evaluate_prompt(args, cwhy.explain_prompt(sys.stdin.read()))
