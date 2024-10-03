import argparse
import subprocess
import sys
from typing import Any

import llm_utils
import openai

from . import conversation, prompts
from .print_debug import dprint, enable_debug_printing


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
        dprint(f"'{args.llm}' either does not exist or you do not have access to it.")
        raise e
    except openai.BadRequestError as e:
        dprint("Something is wrong with your prompt.")
        raise e
    except openai.RateLimitError as e:
        dprint("You have exceeded a rate limit or have no remaining funds.")
        raise e
    except openai.APITimeoutError as e:
        dprint("The API timed out.")
        dprint("You can increase the timeout with the --timeout option.")
        raise e


def evaluate_diff(client: openai.OpenAI, args, stdin):
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


def evaluate(client: openai.OpenAI, args, stdin):
    if args.subcommand == "explain":
        return evaluate_text_prompt(client, args, prompts.explain_prompt(args, stdin))
    elif args.subcommand == "diff":
        completion = evaluate_diff(client, args, stdin)
        tool_calls = completion.choices[0].message.tool_calls
        assert len(tool_calls) == 1
        return tool_calls[0].function.arguments
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

    if args.debug:
        enable_debug_printing()

    if args.show_prompt:
        dprint("===================== Prompt =====================")
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
        client = openai.OpenAI()
        result = evaluate(
            client, args, process.stderr if process.stderr else process.stdout
        )
        dprint(result)
    except openai.OpenAIError as e:
        dprint(str(e).strip())
    dprint("==================================================")

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
    dprint(msg)
    dprint("-" * len(msg))
    text: str = completion.choices[0].message.content

    if wrap:
        text = llm_utils.word_wrap_except_code_blocks(text)

    text += "\n\n"
    text += f"(TODO seconds, $TODO USD.)"

    return text
