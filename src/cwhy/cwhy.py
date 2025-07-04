import argparse
import subprocess
import sys


def main(args: argparse.Namespace) -> None:
    process = subprocess.run(
        args.command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )

    if process.returncode == 0:
        return

    stdin = process.stdout

    if args.show_prompt:
        from . import prompts

        print("===================== Prompt =====================")
        print(prompts.explain_prompt(args, stdin))
        print("==================================================")
        sys.exit(0)

    print(process.stdout)
    print("==================================================")
    print(f"CWhy ({args.llm})")
    print("==================================================")

    from . import explain

    explain.explain(args, stdin)

    print("==================================================")

    sys.exit(process.returncode)
