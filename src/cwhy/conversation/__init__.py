import json
import textwrap

import litellm  # type: ignore
import llm_utils

from . import utils
from .diff_functions import DiffFunctions
from .explain_functions import ExplainFunctions


def converse(args, diagnostic):
    fns = ExplainFunctions(args)
    available_functions_names = [fn["function"]["name"] for fn in fns.as_tools()]
    system_message = textwrap.dedent(
        f"""
            You are an assistant debugger. The user is having an issue with their code, and you are trying to help them.
            A few functions exist to help with this process, namely: {", ".join(available_functions_names)}.
            Don't hesitate to call as many functions as needed to give the best possible answer.
            Once you have identified the problem, explain the diagnostic and provide a way to fix the issue if you can.
        """
    ).strip()
    user_message = f"Here is my error message:\n\n```\n{utils.get_truncated_error_message(args, diagnostic)}\n```\n\nWhat's the problem?"
    conversation = [
        {"role": "system", "content": system_message},
        {"role": "user", "content": user_message},
    ]

    while True:
        completion = litellm.completion(
            model=args.llm,
            messages=conversation,
            tools=fns.as_tools(),
        )

        choice = completion.choices[0]
        if choice.finish_reason == "tool_calls":
            for tool_call in choice.message.tool_calls:
                function_response = fns.dispatch(tool_call.function)
                if function_response:
                    conversation.append(choice.message)
                    conversation.append(
                        {
                            "tool_call_id": tool_call.id,
                            "role": "tool",
                            "name": tool_call.function.name,
                            "content": function_response,
                        }
                    )
            print()
        elif choice.finish_reason == "stop":
            text = completion.choices[0].message.content
            return llm_utils.word_wrap_except_code_blocks(text)
        else:
            print(f"Not found: {choice.finish_reason}.")


def diff_converse(args, diagnostic):
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

    pick_action_schema = {
        "name": "pick_action",
        "description": "Picks an action to take to get more information about or fix the code.",
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
    }

    while True:
        # 1. Pick an action.
        completion = litellm.completion(
            model=args.llm,
            messages=conversation,
            tools=[{"type": "function", "function": pick_action_schema}],
            tool_choice={
                "type": "function",
                "function": {"name": "pick_action"},
            },
        )

        fn = completion.choices[0].message.tool_calls[0].function
        arguments = json.loads(fn.arguments)
        action = arguments["action"]

        tool = [t for t in tools if t["function"]["name"] == action][0]
        completion = litellm.completion(
            model=args.llm,
            messages=conversation,
            tools=[tool],
            tool_choice={
                "type": "function",
                "function": {"name": tool["function"]["name"]},
            },
        )

        choice = completion.choices[0]
        tool_call = choice.message.tool_calls[0]
        function_response = fns.dispatch(tool_call.function)
        if function_response:
            conversation.append(choice.message)
            conversation.append(
                {
                    "tool_call_id": tool_call.id,
                    "role": "tool",
                    "name": tool_call.function.name,
                    "content": function_response,
                }
            )

        print()
