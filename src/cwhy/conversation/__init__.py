import json
import textwrap

import openai

from . import utils
from .diff_functions import DiffFunctions


def diff_converse(client: openai.OpenAI, args, diagnostic):
    fns = DiffFunctions(args)
    tools = fns.as_tools()
    tool_names = [fn["function"]["name"] for fn in tools]
    system_message = textwrap.dedent(
        f"""
            You are an assistant programmer. The user is having an issue with their code, and you are trying to help them fix the code.
            You may only call the following available functions: {", ".join(tool_names)}.
            Your task is done only when the program can successfully compile and/or run, call as many functions as needed to reach this goal.
        """
    ).strip()
    user_message = f"Here is my error message:\n\n```\n{utils.get_truncated_error_message(args, diagnostic)}\n```\n\nPlease help me fix it."
    conversation = [
        {"role": "system", "content": system_message},
        {"role": "user", "content": user_message},
    ]

    while True:
        # 1. Pick an action.
        completion = client.chat.completions.create(  # type: ignore
            model=args.llm,
            messages=conversation,
            tools=[
                {
                    "type": "function",
                    "function": {
                        "name": "pick_action",
                        "description": "Picks an action to get more information about the code or fix it.",
                        "parameters": {
                            "type": "object",
                            "properties": {
                                "action": {
                                    "type": "string",
                                    "enum": tool_names,
                                },
                            },
                            "required": ["action"],
                        },
                    },
                }
            ],
            tool_choice={"type": "function", "function": {"name": "pick_action"}},
            timeout=args.timeout,
        )

        assert completion.choices and len(completion.choices) == 1
        choice = completion.choices[0]
        assert choice.message.tool_calls and len(choice.message.tool_calls) == 1
        fn = choice.message.tool_calls[0].function
        arguments = json.loads(fn.arguments)
        action = arguments["action"]

        tool = [t for t in tools if t["function"]["name"] == action][0]
        completion = client.chat.completions.create(  # type: ignore
            model=args.llm,
            messages=conversation,
            tools=[tool],
            tool_choice={
                "type": "function",
                "function": {"name": tool["function"]["name"]},
            },
            timeout=args.timeout,
        )

        assert completion.choices and len(completion.choices) == 1
        choice = completion.choices[0]
        assert choice.message.tool_calls and len(choice.message.tool_calls) == 1
        tool_call = choice.message.tool_calls[0]
        function_response = fns.dispatch(tool_call.function)
        if function_response:
            conversation.append(choice.message)
            conversation.append(
                {
                    "tool_call_id": tool_call.id,
                    "role": "tool",
                    "content": function_response,
                }
            )

        print()
