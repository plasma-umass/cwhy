import argparse
import subprocess
import sys
from typing import Any

import llm_utils
import openai

from . import conversation, prompts


def complete(
    client: openai.OpenAI, args: argparse.Namespace, user_prompt: str, **kwargs: Any
):
    try:
        completion = client.chat.completions.create(
            model=args.llm,
            messages=[{"role": "user", "content": user_prompt}],
            timeout=args.timeout,
            **kwargs,
        )
        return completion
    except openai.NotFoundError as e:
        print(f"'{args.llm}' either does not exist or you do not have access to it.")
        raise e
    except openai.BadRequestError as e:
        print("Something is wrong with your prompt.")
        raise e
    except openai.RateLimitError as e:
        print("You have exceeded a rate limit or have no remaining funds.")
        raise e
    except openai.APITimeoutError as e:
        print("The API timed out.")
        print("You can increase the timeout with the --timeout option.")
        raise e


def evaluate(client: openai.OpenAI, args: argparse.Name, stdin: str) -> str:
    if args.subcommand == "explain":
        return evaluate_text_prompt(client, args, prompts.explain_prompt(args, stdin))
    elif args.subcommand == "diff-converse":
        return conversation.diff_converse(client, args, stdin)
    else:
        raise Exception(f"unknown subcommand: {args.subcommand}")


def main(args: argparse.Namespace) -> None:
    process = subprocess.run(
        args.command,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    if process.returncode == 0:
        return

    if args.show_prompt:
        print("===================== Prompt =====================")
        if args.subcommand == "explain":
            print(prompts.explain_prompt(args, process.stderr))
        print("==================================================")
        sys.exit(0)

    print(process.stdout)
    print(process.stderr, file=sys.stderr)
    print("==================================================")
    print("CWhy")
    print("==================================================")
    try:
        client = openai.OpenAI()
        result = evaluate(
            client, args, process.stderr if process.stderr else process.stdout
        )
        print(result)
    except openai.OpenAIError as e:
        print(str(e).strip())
    print("==================================================")

    sys.exit(process.returncode)


def evaluate_text_prompt(
    client: openai.OpenAI,
    args: argparse.Namespace,
    prompt: str,
    wrap: bool = True,
    **kwargs: Any,
) -> str:
    completion = complete(client, args, prompt, **kwargs)

    msg = f"Analysis from {args.llm}:"
    print(msg)
    print("-" * len(msg))
    text: str = completion.choices[0].message.content

    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    text += "\n\n"
    text += f"(TODO seconds, $TODO USD.)"

    return text
