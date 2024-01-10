import argparse
import os
import subprocess
import sys

import yaml

from . import prepare

ROOT = os.path.dirname(os.path.abspath(__file__))


def get_cwhy_prompt(invocation):
    process = subprocess.run(
        f"cwhy --show-prompt --- {invocation}",
        text=True,
        shell=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        cwd=ROOT,
    )
    stdout, stderr = process.stdout, process.stderr
    if stderr.strip():
        print("CWhy reported an error or warning.")
        print(stderr.strip())
        sys.exit(1)
    return stdout


def main(args):
    prepare.clean()
    prepare.prepare_all()

    with open(os.path.join(ROOT, "manifest.yml"), "r") as stream:
        manifest = yaml.load(stream, yaml.Loader)

    for name, language in manifest.items():
        path = language["path"]
        extension = language["extension"]
        if args.platform not in language["compilers"]:
            continue
        compilers = language["compilers"][args.platform]
        tests = sorted(
            [
                f
                for f in os.listdir(os.path.join(ROOT, path))
                if os.path.isfile(os.path.join(ROOT, path, f)) and f.endswith(extension)
            ]
        )
        for compiler, invocation in compilers.items():
            directory = os.path.join(ROOT, ".regression", args.platform, compiler)
            for test in tests:
                test_invocation = invocation.format(
                    DEPENDENCIES_INSTALL=os.path.join(
                        ROOT, "_deps", path, test, "install"
                    ),
                    FILENAME=os.path.join(ROOT, path, test),
                )
                prompt = get_cwhy_prompt(test_invocation)
                savefile = os.path.join(directory, test)

                if args.generate:
                    os.makedirs(directory, exist_ok=True)
                    with open(savefile, "w") as save:
                        save.write(prompt)
                elif args.check:
                    if not os.path.isfile(savefile):
                        print(
                            f"::warning file={args.platform}/{compiler[0]}/{path}/{test}::Save file does not exist."
                        )
                        continue

                    with open(savefile, "r") as save:
                        saved = save.read()
                        if saved != prompt:
                            print(
                                f"Prompt for {args.platform}/{compiler[0]}/{path}/{test} has changed."
                            )
                            print(f"Saved prompt:")
                            print(saved)
                            print()
                            print("New prompt:")
                            print(prompt)
                            sys.exit(1)
                else:
                    assert False, "Unreachable."


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--platform", choices=["ubuntu", "windows", "macos"], required=True
    )
    commands = parser.add_mutually_exclusive_group(required=True)
    commands.add_argument("--generate", action="store_true")
    commands.add_argument("--check", action="store_true")
    main(parser.parse_args())
