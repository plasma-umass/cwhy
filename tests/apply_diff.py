#! /usr/bin/env python3

import json
import sys


def apply(data):
    for modification in data["diff"]["modifications"]:
        with open(modification["filename"], "r") as f:
            lines = [line.rstrip() for line in f.readlines()]
        lines = (
            lines[: modification["start-line-number"] - 1]
            + modification["replacement"].splitlines()
            + lines[
                modification["start-line-number"]
                + modification["number-lines-remove"]
                - 1 :
            ]
        )
        with open(modification["filename"], "w") as f:
            f.write("\n".join(lines))


if __name__ == "__main__":
    apply(json.load(sys.stdin))
