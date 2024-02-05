import argparse

import llm_utils


def get_truncated_error_message(args: argparse.Namespace, diagnostic: str) -> str:
    """
    Alternate taking front and back lines until the maximum number of tokens.
    """
    front: list[str] = []
    back: list[str] = []
    diagnostic_lines = diagnostic.splitlines()
    n = len(diagnostic_lines)

    def build_diagnostic_string() -> str:
        return "\n".join(front) + "\n\n[...]\n\n" + "\n".join(reversed(back)) + "\n"

    for i in range(n):
        if i % 2 == 0:
            line = diagnostic_lines[i // 2]
            list = front
        else:
            line = diagnostic_lines[n - i // 2 - 1]
            list = back
        list.append(line)
        count = llm_utils.count_tokens(args.llm, build_diagnostic_string())
        if count > args.max_error_tokens:
            list.pop()
            break
    return build_diagnostic_string()
