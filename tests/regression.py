import argparse
import os
import subprocess

ROOT = os.path.dirname(os.path.abspath(__file__))
LANGUAGES = [
    {
        "name": "C++",
        "path": "c++",
        "extension": ".cpp",
        "compilers": {
            "ubuntu": [["g++-12"], ["clang++-17"]],
            "macos": [["clang++", "-std=c++20"]],
        },
    }
]


def get_diagnostic(compiler, path):
    return subprocess.run(
        [*compiler, path],
        text=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
        cwd=ROOT,
    ).stderr


def get_cwhy_prompt(compiler, path):
    diagnostic = get_diagnostic(compiler, path)
    process = subprocess.Popen(
        ["cwhy", "--show-prompt"],
        text=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = process.communicate(diagnostic)
    assert not stderr.strip(), "CWhy reports an error or warning."
    return stdout


def main(args):
    for language in LANGUAGES:
        name = language["name"]
        path = language["path"]
        extension = language["extension"]
        if args.platform not in language["compilers"]:
            continue
        compilers = language["compilers"][args.platform]
        for compiler in compilers:
            tests = sorted(
                [
                    f
                    for f in os.listdir(os.path.join(ROOT, path))
                    if os.path.isfile(os.path.join(ROOT, path, f))
                    and f.endswith(extension)
                ]
            )
            directory = os.path.join(ROOT, ".regression", args.platform, compiler[0])
            for test in tests:
                prompt = get_cwhy_prompt(compiler, os.path.join(ROOT, path, test))
                savefile = os.path.join(directory, test)

                if args.generate:
                    os.makedirs(directory, exist_ok=True)
                    with open(savefile, "w") as save:
                        save.write(prompt)
                elif args.check:
                    assert os.path.isfile(
                        savefile
                    ), f"Save file for {path}/{test} does not exist."
                    with open(savefile, "r") as save:
                        assert (
                            save.read() == prompt
                        ), f"Prompt for {path}/{test} has changed."
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
