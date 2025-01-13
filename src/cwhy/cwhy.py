import argparse
import subprocess
import sys
import time

import llm_utils
import openai

from . import conversation, prompts


def complete(client: openai.OpenAI, args: argparse.Namespace, user_prompt: str):
    try:
        completion = client.chat.completions.create(
            model=args.llm,
            messages=[{"role": "user", "content": user_prompt}],
            timeout=args.timeout,
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


def evaluate(client: openai.OpenAI, args: argparse.Namespace, stdin: str) -> str:
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

    # TODO: Remove this after issuing next release.
    if "/" in args.llm:
        print(
            "[CWHY WARNING] CWhy went back to using the OpenAI API only (not LiteLLM).",
            f"[CWHY WARNING] Please specify the model name directly without any prefix, i.e. {args.llm[args.llm.index('/') + 1:]}.",
            "[CWHY WARNING] If you were using AWS, you can specify OPENAI_BASE_URL instead.",
            "[CWHY WARNING] See updated documentation at https://github.com/plasma-umass/cwhy.",
            file=sys.stderr,
            sep="\n",
        )
        args.llm = args.llm[args.llm.index("/") + 1 :]

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
    client: openai.OpenAI, args: argparse.Namespace, prompt: str, wrap: bool = True
) -> str:
    start = time.time()
    completion = complete(client, args, prompt)
    end = time.time()


    text: str = completion.choices[0].message.content
    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    text += "\n\n"
    text += f"({end - start:.1f} seconds, "
    text += f"{completion.usage.prompt_tokens} prompt tokens, "
    text += f"{completion.usage.completion_tokens} completion tokens.)"

    return text
