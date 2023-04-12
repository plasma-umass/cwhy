#! /usr/bin/env python3
import asyncio
import click
import importlib.metadata

from . import cwhy

def evaluate_prompt(ctx, prompt, wrap = True):
    if not prompt:
        # Do nothing if nothing was sent to stdin
        return
    if ctx.obj['show-prompt']:
        print("===================== Prompt =====================")
        print(prompt)
        print("==================================================")
    text = asyncio.run(cwhy.complete(ctx, prompt))
    if wrap: text = cwhy.word_wrap_except_code_blocks(text)
    print(text)

@click.group(invoke_without_command = True)
@click.option('--llm', type = click.Choice(['gpt-4', 'gpt-3.5-turbo']), required = False, default = 'gpt-3.5-turbo')
@click.option('--show-prompt', is_flag = True, help = "Print the prompt before sending it to OpenAI for debugging.", required = False, default = False)
@click.option('--timeout', type = int, required = False, default = 60, help = 'Timeout for API calls in seconds.')
@click.option('--version', is_flag=True, help = "Print the version of cwhy.", required = False, default = False)
@click.pass_context
def main(ctx, llm, timeout, version, show_prompt):
    if version:
        print(f"cwhy version {importlib.metadata.metadata('cwhy')['Version']}")
        return
    ctx.ensure_object(dict)
    ctx.obj['llm'] = llm
    ctx.obj['timeout'] = timeout
    ctx.obj['show-prompt'] = show_prompt

    if ctx.invoked_subcommand is None:
        ctx.invoke(explain)

@main.command(short_help = 'Explain the diagnostic.')
@click.pass_context
def explain(ctx):
    evaluate_prompt(ctx, cwhy.explain_prompt())

@main.command(short_help = 'Propose a fix for the diagnostic.')
@click.pass_context
def fix(ctx):
    evaluate_prompt(ctx, cwhy.fix_prompt())

@main.command(short_help = 'Extract the source locations from the diagnostic as CSV.')
@click.pass_context
def extract_sources(ctx):
    evaluate_prompt(ctx, cwhy.extract_sources_prompt(), wrap = False)

