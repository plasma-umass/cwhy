#! /usr/bin/env python3

import argparse
import importlib.metadata
import os
import platform
import sys
import tempfile
import textwrap
from typing import Any, Optional

from rich.console import Console

from . import cwhy


def py_wrapper(args: argparse.Namespace) -> str:
    return (
        textwrap.dedent(
            f"""
                #! {sys.executable}
                import os
                import sys
                if "CWHY_DISABLE" in os.environ:
                    import subprocess
                    sys.exit(subprocess.run([*{args.command}, *sys.argv[1:]]).returncode)
                else:
                    import argparse
                    from cwhy import cwhy
                    args = argparse.Namespace(**{vars(args)})
                    args.command.extend(sys.argv[1:])
                    cwhy.main(args)
            """
        ).strip()
        + "\n"
    )


def win_wrapper(python_filename: str) -> str:
    return (
        textwrap.dedent(
            f"""
                @echo off
                python {python_filename} %*
            """
        ).strip()
        + "\n"
    )


class RichArgParser(argparse.ArgumentParser):
    def __init__(self, *args: Any, **kwargs: Any):
        self.console = Console(highlight=False)
        super().__init__(*args, **kwargs)

    def _print_message(self, message: Optional[str], file: Any = None) -> None:
        if message:
            self.console.print(message)


class CWhyArgumentFormatter(argparse.HelpFormatter):
    # RawDescriptionHelpFormatter.
    def _fill_text(self, text, width, indent):  # type: ignore
        return "".join(indent + line for line in text.splitlines(keepends=True))

    # RawTextHelpFormatter.
    def _split_lines(self, text, width):  # type: ignore
        return text.splitlines()

    # ArgumentDefaultsHelpFormatter.
    # Ignore if help message is multiline.
    def _get_help_string(self, action):  # type: ignore
        help = action.help
        if "\n" not in help and "%(default)" not in action.help:
            if action.default is not argparse.SUPPRESS:
                defaulting_nargs = [argparse.OPTIONAL, argparse.ZERO_OR_MORE]
                if action.option_strings or action.nargs in defaulting_nargs:
                    help += " (default: %(default)s)"
        return help


def main() -> None:
    description = textwrap.dedent(
        r"""
            [b]CWhy[/b]: Explains and proposes fixes for compile-time errors for many programming languages.
            [blue][link=https://github.com/plasma-umass/cwhy]https://github.com/plasma-umass/cwhy[/link][/blue]

            usage:
                [b]cwhy \[OPTIONS...] --- COMMAND...[/b]
            usage (GNU Make):
                [b]CXX=`cwhy --wrapper \[OPTIONS...] --- c++` make[/b]
            usage (CMake):
                [b]cmake -DCMAKE_CXX_COMPILER=`cwhy --wrapper \[OPTIONS...] --- c++`[/b]
        """
    ).strip()

    parser = RichArgParser(
        prog="cwhy",
        usage=argparse.SUPPRESS,
        description=description,
        formatter_class=CWhyArgumentFormatter,
    )

    parser.add_argument(
        "--version",
        action="version",
        version=f"%(prog)s v{importlib.metadata.metadata('cwhy')['Version']}",
        default=argparse.SUPPRESS,
        help="print the version of CWhy and exit",
    )

    parser.add_argument(
        "--llm",
        type=str,
        default="gpt-4o-mini",
        help="the language model to use",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=60,
        help="the timeout for API calls in seconds",
    )
    # The default maximum context length for `gpt-3.5-turbo` is 4096 tokens.
    # We keep 256 tokens for other parts of the prompt, and split the remainder in two
    # for the error message and code sections, resulting in 1920 tokens for each.
    parser.add_argument(
        "--max-error-tokens",
        type=int,
        default=1920,
        help="the maximum number of tokens from the error message to send in the prompt",
    )
    parser.add_argument(
        "--max-code-tokens",
        type=int,
        default=1920,
        help="the maximum number of code locations tokens to send in the prompt",
    )

    parser.add_argument(
        "--show-prompt",
        action="store_true",
        help="when enabled, only print prompt and exit (for debugging purposes)",
    )
    parser.add_argument(
        "--wrapper",
        action="store_true",
        help="generate a temporary executable used to wrap to compiler command",
    )
    parser.add_argument(
        "---",
        dest="command",
        required=True,
        help=argparse.SUPPRESS,
        nargs=argparse.REMAINDER,
    )

    args = parser.parse_args()

    if not args.wrapper:
        cwhy.main(args)
        return

    with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".py") as f:
        f.write(py_wrapper(args))
    if platform.system() == "Windows":
        # We need to make a second wrapper, wrapping the Python script in a shell script.
        with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".bat") as g:
            g.write(win_wrapper(f.name))
            print(g.name.replace("\\", "/"))
    else:
        # NamedTemporaryFiles are not executable by default.
        # Set its mode to 755 here with an octal literal.
        os.chmod(f.name, 0o755)
        print(f.name)


if __name__ == "__main__":
    main()
