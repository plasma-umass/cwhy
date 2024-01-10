#! /usr/bin/env python3

import argparse
import asyncio
import os
import subprocess

import openai

from cwhy import cwhy

from . import anonymizer

ROOT = os.path.dirname(os.path.abspath(__file__))

LANGUAGES = {
    "C++": {
        "path": "c++",
        "compiler": [os.environ.get("CXX", "c++"), "-x", "c++"],
    },
}

client = openai.AsyncOpenAI()


def verify_prompt(answer, choice):
    prompt = "I have used two different models to help me debug a program.\n"
    prompt += "The first model gave this output, which helped me solve the error.\n"
    prompt += "````\n" + answer + "\n````\n"
    prompt += "The second model gave this output.\n"
    prompt += "````\n" + choice + "\n````\n"
    prompt += "Would you say the the second model's answer is correct and matches with the first model?\n"
    prompt += "Start your answer with 'true' or 'false'."
    return prompt


async def evaluate_verifications(args, prompts):
    completions = await asyncio.gather(
        *[
            client.chat.completions.create(
                model=args["verification_llm"],
                request_timeout=args["timeout"],
                messages=[{"role": "user", "content": prompt}],
            )
            for prompt in prompts
        ]
    )

    successes = 0
    for prompt, completion in zip(prompts, completions):
        text = completion.choices[0].message.content
        if text.lower().startswith("true"):
            successes += 1
        else:
            print(f"{'=' * 34} Fail {'=' * 34}")
            print(prompt)
            print("-" * 74)
            print(text)
            print("=" * 74)

    return successes / len(completions)


async def evaluate_benchmark(args, language, benchmark):
    filename, answer = anonymizer.anonymize(
        os.path.join(ROOT, LANGUAGES[language]["path"], benchmark)
    )
    if answer is None:
        print(f"\t{benchmark}: skipped")
        return

    process = subprocess.run(
        [*LANGUAGES[language]["compiler"], filename], stderr=subprocess.PIPE
    )
    compiler_output = process.stderr.decode("utf-8")

    completion = await aclient.chat.completions.create(
        model=args["llm"],
        request_timeout=args["timeout"],
        n=args["n"],
        messages=[
            {"role": "user", "content": cwhy.explain_prompt(args, compiler_output)}
        ],
    )

    success_rate = await evaluate_verifications(
        args,
        [
            verify_prompt(answer, choice.message.content)
            for choice in completion.choices
        ],
    )
    print(f"\t{benchmark}: {100 * success_rate:.2f}%")

    os.remove(filename)


async def evaluate_language(args, language):
    benchmarks = [
        b for b in os.listdir(os.path.join(ROOT, LANGUAGES[language]["path"]))
    ]
    benchmarks.sort()

    await asyncio.gather(
        *[evaluate_benchmark(args, language, benchmark) for benchmark in benchmarks]
    )


def main(args):
    print(f"{'=' * 28} CWhy Test Runner {'=' * 28}")
    print(f"LLM              : {args['llm']}")
    print(f"Timeout          : {args['timeout']}")
    print(f"Max Error Tokens : {args['max_error_tokens']}")
    print(f"Max Code Tokens  : {args['max_code_tokens']}")
    print()
    print(f"Verification LLM : {args['verification_llm']}")
    print(f"Iterations       : {args['n']}")

    for language in LANGUAGES.keys():
        print(f"[{language}] {'-' * (71 - len(language))}")
        asyncio.run(evaluate_language(args, language))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    # These parameters will be passed directly to CWhy, hence they should match.
    parser.add_argument("--llm", type=str, default="gpt-4")
    parser.add_argument("--timeout", type=int, default=180)
    parser.add_argument("--max-error-tokens", type=int, default=1920)
    parser.add_argument("--max-code-tokens", type=int, default=1920)

    # These arguments are specific to the test runner.
    # They should not clash with any existing CWhy parameters.
    parser.add_argument("--verification-llm", type=str, default="gpt-4")
    parser.add_argument("-n", type=int, default=10)

    main(vars(parser.parse_args()))
