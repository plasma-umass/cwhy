#! /usr/bin/env python3

# Do NOT run this script as root. It may edit standard library headers.

import argparse
import os
import subprocess
import json

from cwhy import cwhy

from . import anonymizer, apply_diff

ROOT = os.path.dirname(os.path.abspath(__file__))

LANGUAGES = {
    "C++": {
        "path": "c++",
        "compiler": [os.environ.get("CXX", "c++"), "-x", "c++", "-shared"],
    },
    "Python": {
        "path": "python",
        "compiler": ["python3"],
    },
}


def evaluate_once(args):
    filename, _ = anonymizer.anonymize(
        os.path.join(ROOT, LANGUAGES[args["language"]]["path"], args["benchmark"])
    )

    print(f"Trying to repair {args['benchmark']} ({filename})...")

    process = subprocess.run(
        [*LANGUAGES[args["language"]]["compiler"], filename],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
    )
    status = process.returncode
    compiler_output = process.stderr.decode("utf-8")

    usage = []

    while status != 0:
        if len(usage) == args["max_retries"]:
            break

        completion = cwhy.evaluate_diff(args, compiler_output)
        usage.append(completion["usage"])
        try:
            apply_diff.apply(
                json.loads(completion.choices[0].message.function_call.arguments)
            )
        except FileNotFoundError:
            break
        except PermissionError:
            break
        except json.decoder.JSONDecodeError:
            break

        process = subprocess.run(
            [*LANGUAGES[args["language"]]["compiler"], filename],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE,
        )
        status = process.returncode
        compiler_output = process.stderr.decode("utf-8")

    if not args["keep_temporary_files"]:
        os.remove(filename)
    return status == 0, usage


def evaluate_benchmark(args):
    successes = 0
    for _ in range(args["n"]):
        success, usage = evaluate_once(args)
        usage = [u["total_tokens"] for u in usage]
        if success:
            successes += 1
            print(
                f"Success in {len(usage)} retries! Total tokens used per retry: {usage}"
            )
        else:
            print(
                f"Failed after {len(usage)} retries. Total tokens used per retry: {usage}"
            )
    print(
        f"\n{args['language']}/{args['benchmark']} success rate: {100 * successes / args['n']:.2f}%"
    )


def main(args):
    print(f"{'=' * 28} CWhy Test Runner {'=' * 28}")
    print(f"LLM              : {args['llm']}")
    print(f"Timeout          : {args['timeout']}")
    print(f"Max Error Tokens : {args['max_error_tokens']}")
    print(f"Max Code Tokens  : {args['max_code_tokens']}")
    print()
    print(f"Max retries      : {args['max_retries']}")
    print(f"Iterations       : {args['n']}")
    print(f"Benchmark        : {args['language']}/{args['benchmark']}")
    print("=" * 74)

    evaluate_benchmark(args)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    # These parameters will be passed directly to CWhy, hence they should match.
    parser.add_argument("--llm", type=str, default="gpt-4")
    parser.add_argument("--timeout", type=int, default=180)
    parser.add_argument("--max-error-tokens", type=int, default=1920)
    parser.add_argument("--max-code-tokens", type=int, default=1920)

    # These arguments are specific to the test runner.
    # They should not clash with any existing CWhy parameters.
    parser.add_argument("--keep-temporary-files", action="store_true")
    parser.add_argument("--max-retries", type=int, default=5)
    parser.add_argument("-n", type=int, default=10)
    parser.add_argument("language", type=str, choices=LANGUAGES.keys())
    parser.add_argument("benchmark", type=str)

    main(vars(parser.parse_args()))
