#! /usr/bin/env python3

import json
import sys


def apply(data):
    # Sort modifications by reverse start line number to apply them in that order.
    data["modifications"].sort(key=lambda m: m["start-line-number"], reverse=True)

    for modification in data["modifications"]:
        with open(modification["filename"], "r") as f:
            lines = [line.rstrip() for line in f.readlines()]

        pre_lines = lines[: modification["start-line-number"] - 1]
        replacement_lines = modification["replacement"].splitlines()
        post_lines = lines[
            modification["start-line-number"]
            + modification["number-lines-remove"]
            - 1 :
        ]

        # If replacing a single line, make sure we keep indentation.
        if (
            modification["number-lines-remove"] == 1
            and len(replacement_lines) == 1
            and modification["start-line-number"] >= 1
        ):
            replaced_line = lines[modification["start-line-number"] - 1]
            replacement_lines[0] = replacement_lines[0].lstrip()
            n = len(replaced_line) - len(replaced_line.lstrip())
            whitespace = replaced_line[:n]
            replacement_lines[0] = whitespace + replacement_lines[0]

        lines = pre_lines + replacement_lines + post_lines
        with open(modification["filename"], "w") as f:
            f.write("\n".join(lines))


if __name__ == "__main__":
    apply(json.load(sys.stdin))
