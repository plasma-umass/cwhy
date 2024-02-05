import argparse
import contextlib
import os
import subprocess
import sys

import litellm # type: ignore
import llm_utils

litellm.set_verbose=False

# Turn off most logging
from llm_utils import logging
logging.getLogger().setLevel(logging.ERROR)

from openai import NotFoundError, RateLimitError, APITimeoutError, OpenAIError, BadRequestError

from . import conversation, prompts

def print_key_info():
    print("You need a key (or keys) from an AI service to use CWhy.")
    print()
    print("OpenAI:")
    print("  You can get a key here: https://platform.openai.com/api-keys")
    print("  Set the environment variable OPENAI_API_KEY to your key value:")
    print("    export OPENAI_API_KEY=<your key>")
    print()
    print("Bedrock:")
    print("  To use Bedrock, you need an AWS account.")
    print("  Set the following environment variables:")
    print("    export AWS_ACCESS_KEY_ID=<your key id>")
    print("    export AWS_SECRET_ACCESS_KEY=<your secret key>")
    print("    export AWS_REGION_NAME=us-west-2")

# If keys are defined in the environment, we use the appropriate service.
AI_service = None
_DEFAULT_FALLBACK_MODELS = []

with contextlib.suppress(KeyError):
    if os.environ["OPENAI_API_KEY"]:
        AI_service = "OpenAI"
        _DEFAULT_FALLBACK_MODELS = ["openai/gpt-4", "openai/gpt-3.5-turbo"]
with contextlib.suppress(KeyError):
    if not _DEFAULT_FALLBACK_MODELS:
        if os.environ["AWS_ACCESS_KEY_ID"] and os.environ["AWS_SECRET_ACCESS_KEY"] and os.environ["AWS_REGION_NAME"]:
            AI_service = "Bedrock"
            _DEFAULT_FALLBACK_MODELS = ["bedrock/anthropic.claude-v2:1"]

# If no AI service was available given the environment variables, print key info and exit.
if not AI_service:
    print_key_info()
    sys.exit(1)
       
def complete(client, args, user_prompt, **kwargs):
    try:
        completion = litellm.completion(
            model=args.llm,
            messages=[{"role": "user", "content": user_prompt}],
            **kwargs,
        )
        return completion
    except NotFoundError as e:
        print(f"'{args.llm}' either does not exist or you do not have access to it.")
        raise e
    except BadRequestError as e:
        print("Something is wrong with your prompt.")
        raise e
    except RateLimitError as e:
        print("You have exceeded a rate limit or have no remaining funds.")
        raise e
    except APITimeoutError as e:
        print("The API timed out.")
        print("You can increase the timeout with the --timeout option.")
        raise e


def evaluate_diff(client, args, stdin):
    prompt = prompts.diff_prompt(args, stdin)
    completion = complete(
        client,
        args,
        prompt,
        tools=[
            {
                "type": "function",
                "function": {
                    "name": "apply_modifications",
                    "description": "Applies the given modifications to the source file with the goal of fixing any existing compilation errors.",
                    "parameters": {
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
                    },
                },
            }
        ],
        tool_choice={
            "type": "function",
            "function": {"name": "apply_modifications"},
        },
    )

    return completion


def evaluate_with_fallback(client, args, stdin):
    for i, model in enumerate(_DEFAULT_FALLBACK_MODELS):
        if i != 0:
            print(f"Falling back to {model}...")
        args.llm = model
        try:
            return evaluate(client, args, stdin)
        except NotFoundError:
            continue


def evaluate(client, args, stdin):
    if args.llm == "default":
        return evaluate_with_fallback(client, args, stdin)

    if args.subcommand == "explain":
        return evaluate_text_prompt(client, args, prompts.explain_prompt(args, stdin))
    elif args.subcommand == "diff":
        completion = evaluate_diff(client, args, stdin)
        tool_calls = completion.choices[0].message.tool_calls
        assert len(tool_calls) == 1
        return tool_calls[0].function.arguments
    elif args.subcommand == "converse":
        return conversation.converse(client, args, stdin)
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
        if args.llm == "default":
            args.llm = _DEFAULT_FALLBACK_MODELS[0]
        if args.subcommand == "explain":
            print(prompts.explain_prompt(args, process.stderr))
        elif args.subcommand == "diff":
            print(prompts.diff_prompt(args, process.stderr))
        print("==================================================")
        sys.exit(0)

    print(process.stdout, end="")
    print(process.stderr, file=sys.stderr, end="")
    print("==================================================")
    print("CWhy")
    print("==================================================")
    try:
        result = evaluate(None, args, process.stderr if process.stderr else process.stdout)
        print(result)
    except OpenAIError:
        print_key_info()
        sys.exit(1)
    print("==================================================")

    sys.exit(process.returncode)


def evaluate_text_prompt(client, args, prompt, wrap=True, **kwargs):

    completion = complete(client, args, prompt, **kwargs)

    msg = f"Analysis from {AI_service}:"
    print(msg)
    print("-" * len(msg))
    text = completion.choices[0].message.content

    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    cost = litellm.completion_cost(completion_response=completion)
    # llm_utils.calculate_cost(
    #    completion.usage.prompt_tokens, completion.usage.completion_tokens, args.llm
    # )
    
    text += "\n\n"
    text += f"(Total cost: approximately ${cost:.2f} USD.)"

    return text
