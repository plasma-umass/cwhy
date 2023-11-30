import os
import subprocess
import sys
import textwrap

import openai

from llm_utils import llm_utils

from .prompts import diff_prompt, explain_prompt, fix_prompt


def complete(client, args, user_prompt, **kwargs):
    if "show_prompt" in args and args.show_prompt:
        print("===================== Prompt =====================")
        print(user_prompt)
        print("==================================================")
        sys.exit(0)

    try:
        completion = client.chat.completions.create(
            model=args.llm,
            messages=[{"role": "user", "content": user_prompt}],
            **kwargs,
        )
        return completion
    except openai.NotFoundError as e:
        print(f"'{args.llm}' either does not exist or you do not have access to it.")
        raise e
    except openai.RateLimitError as e:
        print("You have exceeded a rate limit or have no remaining funds.")
        raise e
    except openai.APITimeoutError as e:
        print("The OpenAI API timed out.")
        print("You can increase the timeout with the --timeout option.")
        raise e


def evaluate_diff(client, args, stdin):
    prompt = diff_prompt(args, stdin)
    completion = complete(
        client,
        args,
        prompt,
        functions=[
            {
                "name": "fix_error",
                "description": "Returns all modifications needed for the provided code to compile.",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "diff": {
                            "type": "object",
                            "properties": {
                                "modifications": {
                                    "type": "array",
                                    "items": {
                                        "type": "object",
                                        "properties": {
                                            "filename": {"type": "string"},
                                            "start-line-number": {"type": "integer"},
                                            "number-lines-remove": {"type": "integer"},
                                            "replacement": {"type": "string"},
                                        },
                                        "required": [
                                            "filename",
                                            "start-line-number",
                                            "number-lines-remove",
                                            "replacement",
                                        ],
                                    },
                                },
                            },
                            "required": ["modifications"],
                        }
                    },
                    "required": ["diff"],
                },
            }
        ],
        function_call={"name": "fix_error"},
    )

    return completion


def evaluate_with_fallback(client, args, stdin):
    DEFAULT_FALLBACK_MODELS = ["gpt-4", "gpt-3.5-turbo"]
    for i, model in enumerate(DEFAULT_FALLBACK_MODELS):
        if i != 0:
            print(f"Falling back to {model}...")
        args.llm = model
        try:
            return evaluate(client, args, stdin)
        except openai.NotFoundError:
            continue


def evaluate(client, args, stdin):
    if args.llm == "default":
        return evaluate_with_fallback(client, args, stdin)

    if args.subcommand == "explain":
        return evaluate_text_prompt(client, args, explain_prompt(args, stdin))
    elif args.subcommand == "fix":
        return evaluate_text_prompt(client, args, fix_prompt(args, stdin))
    elif args.subcommand == "diff":
        return (
            evaluate_diff(client, args, stdin)
            .choices[0]
            .message.function_call.arguments
        )
    else:
        raise Exception(f"unknown subcommand: {args.subcommand}")


def main(args, stdin):
    try:
        client = openai.OpenAI(timeout=args.timeout)
    except openai.OpenAIError:
        print("You need an OpenAI key to use this tool.")
        print("You can get a key here: https://platform.openai.com/account/api-keys")
        print("Set the environment variable OPENAI_API_KEY to your key value.")
        sys.exit(1)

    return evaluate(client, args, stdin)


def evaluate_text_prompt(client, args, prompt, wrap=True, **kwargs):
    completion = complete(client, args, prompt, **kwargs)
    text = completion.choices[0].message.content

    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    cost = llm_utils.calculate_cost(
        completion.usage.prompt_tokens, completion.usage.completion_tokens, args.llm
    )
    text += "\n\n"
    text += f"(Total cost: approximately ${cost:.2f} USD.)"

    return text


def wrapper(args):
    process = subprocess.run(
        [args.wrapper_compiler, *sys.argv[1:]],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )

    status = process.returncode
    if status != 0:
        print(process.stdout)
        if "CWHY_DISABLE" not in os.environ:
            print("==================================================")
            print("CWhy")
            print("==================================================")
            print(evaluate(args, process.stdout))
            print("==================================================")
    sys.exit(status)
