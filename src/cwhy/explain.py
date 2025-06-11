import argparse
import sys

import llm_utils
import openai

from . import prompts
from .functions import Functions


def complete(client: openai.OpenAI, args: argparse.Namespace, prompt: str) -> None:
    fns = Functions(args)
    try:
        conversation: openai.types.responses.ResponseInputParam = [{"role": "user", "content": prompt}]
        while True:
            completion = client.responses.create(
                model=args.llm,
                input=conversation,
                tools=fns.as_tools(),
                timeout=args.timeout,
            )

            for message in completion.output:
                if message.type == "message":
                    if len(message.content) != 1:
                        print(
                            "[WARNING] Unexpected number of message entries: {len(message.content)}"
                        )
                        print("[WARNING] Please report this issue.")
                    for entry in message.content:
                        assert entry.type == "output_text"
                        print(llm_utils.word_wrap_except_code_blocks(entry.text))
                    conversation.append(message)
                elif message.type == "function_call":
                    result = fns.dispatch(message)
                    print()
                    conversation.append(message)
                    conversation.append(
                        {
                            "type": "function_call_output",
                            "call_id": message.call_id,
                            "output": str(result),
                        }
                    )
                else:
                    print(f"[ERROR] Unknown message type: {message.type}")
                    sys.exit(1)

            if len([m for m in completion.output if m.type == "function_call"]) == 0:
                break

        return None
    except openai.NotFoundError as e:
        print(f"'{args.llm}' either does not exist or you do not have access to it.")
        raise e
    except openai.RateLimitError as e:
        print("You have exceeded a rate limit or have no remaining funds.")
        raise e
    except openai.APITimeoutError as e:
        print("The API timed out.")
        print("You can increase the timeout with the --timeout option.")
        raise e


def evaluate(client: openai.OpenAI, args: argparse.Namespace, stdin: str) -> None:
    evaluate_text_prompt(client, args, prompts.explain_prompt(args, stdin))


def explain(args: argparse.Namespace, stdin: str) -> None:
    try:
        client = openai.OpenAI()
        evaluate(client, args, stdin)
    except openai.OpenAIError as e:
        print(str(e).strip())


def evaluate_text_prompt(
    client: openai.OpenAI, args: argparse.Namespace, prompt: str, wrap: bool = True
) -> None:
    usage = complete(client, args, prompt)
    print(usage)
    # text += "\n\n"
    # text += f"({completion.usage.prompt_tokens} prompt tokens, "
    # text += f"{completion.usage.completion_tokens} completion tokens.)"
