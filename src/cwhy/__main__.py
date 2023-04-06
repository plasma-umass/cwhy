#! /usr/bin/env python3
import asyncio
import click
import textwrap

from . import cwhy

@click.command()
@click.option('--fix', is_flag=True, help="Instead of explaining error messages, propose a fix.", required=False, default=False)
def main(fix):
    prompt = cwhy.cwhy_prompt(fix)
    if not prompt:
        # Do nothing if nothing was sent to stdin
        return
    text = asyncio.run(cwhy.complete(prompt))
    if fix:
        print(cwhy.word_wrap_except_code_blocks(text))
    else:
        print('\n'.join(textwrap.wrap(text, width=70)))

