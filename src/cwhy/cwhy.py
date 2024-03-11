import argparse
import os
import subprocess
import sys
import logging

import litellm  # type: ignore
import llm_utils
from openai import (
    NotFoundError,
    RateLimitError,
    APITimeoutError,
    OpenAIError,
    BadRequestError,
)

from . import conversation, prompts
from .print_debug import dprint, enable_debug_printing


# Turn off most logging
litellm.set_verbose = False
logging.getLogger().setLevel(logging.ERROR)


def print_key_info():
    dprint("You need a key (or keys) from an AI service to use CWhy.")
    dprint()
    dprint("OpenAI:")
    dprint("  You can get a key here: https://platform.openai.com/api-keys")
    dprint("  Set the environment variable OPENAI_API_KEY to your key value:")
    dprint("    export OPENAI_API_KEY=<your key>")
    dprint()
    dprint("Bedrock:")
    dprint("  To use Bedrock, you need an AWS account.")
    dprint("  Set the following environment variables:")
    dprint("    export AWS_ACCESS_KEY_ID=<your key id>")
    dprint("    export AWS_SECRET_ACCESS_KEY=<your secret key>")
    dprint("    export AWS_REGION_NAME=us-west-2")
    dprint("  You also need to request access to Claude:")
    dprint(
        "   https://docs.aws.amazon.com/bedrock/latest/userguide/model-access.html#manage-model-access"
    )


_DEFAULT_FALLBACK_MODELS = []

if "OPENAI_API_KEY" in os.environ:
    _DEFAULT_FALLBACK_MODELS = ["openai/gpt-4", "openai/gpt-3.5-turbo"]
elif {
    "AWS_ACCESS_KEY_ID",
    "AWS_SECRET_ACCESS_KEY",
    "AWS_REGION_NAME",
} <= os.environ.keys():
    _DEFAULT_FALLBACK_MODELS = ["bedrock/anthropic.claude-v2:1"]
else:
    print_key_info()
    sys.exit(1)


def complete(args, user_prompt, **kwargs):
    try:
        completion = litellm.completion(
            model=args.llm,
            messages=[{"role": "user", "content": user_prompt}],
            timeout=args.timeout,
            **kwargs,
        )
        return completion
    except NotFoundError as e:
        dprint(f"'{args.llm}' either does not exist or you do not have access to it.")
        raise e
    except BadRequestError as e:
        dprint("Something is wrong with your prompt.")
        raise e
    except RateLimitError as e:
        dprint("You have exceeded a rate limit or have no remaining funds.")
        raise e
    except APITimeoutError as e:
        dprint("The API timed out.")
        dprint("You can increase the timeout with the --timeout option.")
        raise e


def evaluate_diff(args, stdin):
    prompt = prompts.diff_prompt(args, stdin)
    completion = complete(
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


def evaluate_with_fallback(args, stdin):
    for i, model in enumerate(_DEFAULT_FALLBACK_MODELS):
        if i != 0:
            dprint(f"Falling back to {model}...")
        args.llm = model
        try:
            return evaluate(args, stdin)
        except NotFoundError:
            continue


def evaluate(args, stdin):
    if args.llm == "default":
        return evaluate_with_fallback(args, stdin)

    if args.subcommand == "explain":
        return evaluate_text_prompt(args, prompts.explain_prompt(args, stdin))
    elif args.subcommand == "diff":
        completion = evaluate_diff(args, stdin)
        tool_calls = completion.choices[0].message.tool_calls
        assert len(tool_calls) == 1
        return tool_calls[0].function.arguments
    elif args.subcommand == "converse":
        return conversation.converse(args, stdin)
    elif args.subcommand == "diff-converse":
        return conversation.diff_converse(args, stdin)
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

    if args.debug:
        enable_debug_printing()

    if args.show_prompt:
        dprint("===================== Prompt =====================")
        if args.llm == "default":
            args.llm = _DEFAULT_FALLBACK_MODELS[0]
        if args.subcommand == "explain":
            dprint(prompts.explain_prompt(args, process.stderr))
        elif args.subcommand == "diff":
            dprint(prompts.diff_prompt(args, process.stderr))
        dprint("==================================================")
        sys.exit(0)

    dprint(process.stdout)
    dprint(process.stderr, file=sys.stderr)
    dprint("==================================================")
    dprint("CWhy")
    dprint("==================================================")
    try:
        result = evaluate(args, process.stderr if process.stderr else process.stdout)
        dprint(result)
    except OpenAIError:
        print_key_info()
        sys.exit(1)
    dprint("==================================================")

    sys.exit(process.returncode)


def evaluate_text_prompt(args, prompt, wrap=True, **kwargs):
    completion = complete(args, prompt, **kwargs)

    msg = f"Analysis from {args.llm}:"
    dprint(msg)
    dprint("-" * len(msg))
    text = completion.choices[0].message.content

    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    cost = litellm.completion_cost(completion_response=completion)
    text += "\n\n"
    text += f"(Total cost: approximately ${cost:.2f} USD.)"

    return text
