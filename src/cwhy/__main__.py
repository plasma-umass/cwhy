#! /usr/bin/env python3
import asyncio
import click

from . import cwhy

def evaluate_prompt(ctx, prompt, wrap = True):
    if not prompt:
        # Do nothing if nothing was sent to stdin
        return
    if ctx.obj['show-prompt']:
        print("===================== Prompt =====================")
        print(prompt)
        print("==================================================")
    text = asyncio.run(cwhy.complete(prompt))
    if wrap: text = cwhy.word_wrap_except_code_blocks(text)
    print(text)

@click.group(invoke_without_command = True)
@click.option('--show-prompt', is_flag = True, help = "Print the prompt before sending it to OpenAI for debugging.", required = False, default = False)
@click.pass_context
def main(ctx, show_prompt):
    ctx.ensure_object(dict)
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

