import argparse
import asyncio
import os
import re
import subprocess
import tempfile

import openai

from cwhy import cwhy

ROOT = os.path.dirname(os.path.abspath(__file__))

LANGUAGES = {
    "C++": {
        "path": "c++",
        "compiler": [os.environ.get("CXX", "c++"), "-x", "c++"],
    },
}


def verify_prompt(answer, choice):
    prompt = "I have used two different models to help me debug a program.\n"
    prompt += "The first model gave this output, which helped me solve the error.\n"
    prompt += "````\n" + answer + "\n````\n"
    prompt += "The second model gave this output.\n"
    prompt += "````\n" + choice + "\n````\n"
    prompt += "Would you say the the second model's answer is correct and matches with the first model's?\n"
    prompt += "Please only respond with 'true' or 'false' and nothing else."
    return prompt


async def evaluate_verifications(args, prompts):
    completions = await asyncio.gather(
        *[
            openai.ChatCompletion.acreate(
                model=args.llm,
                request_timeout=args.timeout,
                messages=[{"role": "user", "content": prompt}],
            )
            for prompt in prompts
        ]
    )

    successes = 0
    for i in range(len(completions)):
        text = completions[i].choices[0].message.content
        if len(text) < 10 and re.search(r"true", text, re.IGNORECASE):
            successes += 1
        else:
            print(f"{'=' * 34} Fail {'=' * 34}")
            print(prompts[i])
            print("-" * 74)
            print(text)
            print("=" * 74)

    return successes / len(completions)


async def evaluate_benchmark(args, language, benchmark):
    path = os.path.join(ROOT, LANGUAGES[language]["path"], benchmark)
    with open(path) as file:
        content = file.read()

        try:
            i = content.index("/*")
            j = content.index("*/", i)
        except ValueError:
            print(f"\t{benchmark}: skipped")
            return
        answer = content[i + 2 : j].strip()
        code = content[j + 2 :].strip()

    file = tempfile.NamedTemporaryFile(delete=False)
    file.write(code.encode("utf-8"))
    file.close()

    process = subprocess.run(
        [*LANGUAGES[language]["compiler"], file.name], stderr=subprocess.PIPE
    )
    compiler_output = process.stderr.decode("utf-8")

    completion = await openai.ChatCompletion.acreate(
        model=args.llm,
        request_timeout=args.timeout,
        n=args.n,
        messages=[{"role": "user", "content": cwhy.explain_prompt(compiler_output)}],
    )

    success_rate = await evaluate_verifications(
        args,
        [
            verify_prompt(answer, choice.message.content)
            for choice in completion.choices
        ],
    )
    print(f"\t{benchmark}: {100 * success_rate:.2f}%")

    os.remove(file.name)


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
    print("LLM       : " + args.llm)
    print("Timeout   : " + str(args.timeout))
    print("Iterations: " + str(args.n))

    for language in LANGUAGES.keys():
        print(f"[{language}] {'-' * (71 - len(language))}")
        asyncio.run(evaluate_language(args, language))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # The LLM setting is for both CWhy and the verification prompts.
    # Maybe these should be separated.
    parser.add_argument("--llm", type=str, default="gpt-4")
    parser.add_argument("--timeout", type=int, default=180)
    parser.add_argument("-n", type=int, default=10)
    main(parser.parse_args())
