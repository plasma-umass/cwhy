#! /usr/bin/env python3

import argparse
import importlib.metadata
import os
import sys
import tempfile

from . import cwhy

def wrapper(args):
    return f"""#! /usr/bin/env python3

import os
import subprocess
import sys

from cwhy import cwhy

CWHY_CXX = os.environ.get("CWHY_CXX", "c++")

process = subprocess.run(
    [CWHY_CXX, *sys.argv[1:]],
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True,
)
status = process.returncode
if status != 0:
    print(process.stdout)
    print("==================================================")
    print("CWhy")
    print("==================================================")
    cwhy.evaluate_prompt({args}, cwhy.explain_prompt(process.stdout))
    print("==================================================")
sys.exit(status)
"""


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

    args = vars(parser.parse_args())
    
    if args["wrapper"]:
        with tempfile.NamedTemporaryFile(mode="w", delete=False) as f:
            f.write(wrapper(args))
        os.chmod(f.name, 0o755)
        print(f.name)
    elif args["version"]:
        print(f"cwhy version {importlib.metadata.metadata('cwhy')['Version']}")
        return
    else:
        cwhy.evaluate_prompt(args, cwhy.explain_prompt(sys.stdin.read()))
