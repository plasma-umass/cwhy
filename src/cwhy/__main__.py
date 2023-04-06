#! /usr/bin/env python3
import asyncio
import click

from . import cwhy

@click.command()
@click.option('--fix', is_flag=True, help="Instead of explaining error messages, propose a fix.", required=False, default=False)
@click.option('--show-prompt', is_flag=True, help="Print the prompt before sending it to OpenAI for debugging.", required=False, default=False)
def main(fix, show_prompt):
    prompt = cwhy.cwhy_prompt(fix)
    if not prompt:
        # Do nothing if nothing was sent to stdin
        return
    if show_prompt:
        print("===================== Prompt =====================")
        print(prompt)
        print("==================================================")
    text = asyncio.run(cwhy.complete(prompt))
    print(cwhy.word_wrap_except_code_blocks(text))

