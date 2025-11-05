import argparse
import collections
import json
import tempfile
import os
import subprocess


def main(args: argparse.Namespace):

    with open(args.json_file) as f:
        raw_data = json.load(f)
    data = collections.defaultdict(list)
    for base, diff in raw_data:
        data[base].append(diff)

    with open(args.log_file) as f:
        log_lines = f.read().splitlines()

    base_builds = {}
    diff_builds = collections.defaultdict(list)

    i = 0
    while i < len(log_lines):
        assert not log_lines[i].startswith(" ")
        base, builds = log_lines[i].split()
        base_builds[base] = builds == "True"
        i += 1
        while i < len(log_lines) and log_lines[i].startswith(" "):
            _, builds = log_lines[i].strip().split()
            diff_builds[base].append(builds == "True")
            i += 1

    filtered = collections.defaultdict(list)

    for base, builds in base_builds.items():
        if not builds:
            continue

        for diff_index, builds in enumerate(diff_builds[base]):
            if builds:
                continue

            # def display_diff() -> None:
            #     subprocess.Popen(
            #         ["python3", "-m", "ydiff"], text=True, stdin=subprocess.PIPE
            #     ).communicate(data[base][diff_index])

            # display_diff()
            # while answer := input("Does this seem reasonable? [y/n] ") not in [
            #     "y",
            #     "n",
            # ]:
            #     display_diff()

            # if answer == "n":
            #     continue

            filtered[base].append(data[base][diff_index])

    with open(args.output_log_file, "w") as f:
        json.dump(filtered, f)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--json-file", type=str, default="commits.json")
    parser.add_argument("--log-file", type=str, default="commits.log")
    parser.add_argument("--output-log-file", type=str, default="commits-filtered.log")
    main(parser.parse_args())
