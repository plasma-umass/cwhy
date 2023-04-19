#! /usr/bin/env python3
import importlib.metadata
import sys

import click

from . import cwhy


@click.group(invoke_without_command=True)
@click.option(
    "--llm",
    type=click.Choice(["gpt-4", "gpt-3.5-turbo"]),
    required=False,
    default="gpt-3.5-turbo",
)
@click.option(
    "--show-prompt",
    is_flag=True,
    help="Print the prompt before sending it to OpenAI for debugging.",
    required=False,
    default=False,
)
@click.option(
    "--timeout",
    type=int,
    required=False,
    default=60,
    help="Timeout for API calls in seconds.",
)
@click.option(
    "--version",
    is_flag=True,
    help="Print the version of cwhy.",
    required=False,
    default=False,
)
@click.pass_context
def main(ctx, llm, timeout, version, show_prompt):
    if version:
        print(f"cwhy version {importlib.metadata.metadata('cwhy')['Version']}")
        return
    ctx.ensure_object(dict)
    ctx.obj["llm"] = llm
    ctx.obj["timeout"] = timeout
    ctx.obj["show-prompt"] = show_prompt

    if ctx.invoked_subcommand is None:
        ctx.invoke(explain)


@main.command(short_help="Explain the diagnostic.")
@click.pass_context
def explain(ctx):
    cwhy.evaluate_prompt(ctx.obj, cwhy.explain_prompt(sys.stdin.read()))


@main.command(short_help="Propose a fix for the diagnostic.")
@click.pass_context
def fix(ctx):
    cwhy.evaluate_prompt(ctx.obj, cwhy.fix_prompt(sys.stdin.read()))


@main.command(short_help="Extract the source locations from the diagnostic as CSV.")
@click.pass_context
def extract_sources(ctx):
    cwhy.evaluate_prompt(
        ctx.obj,
        cwhy.extract_sources_prompt(sys.stdin.read()),
        wrap=False,
    )
