import asyncio
import textwrap

from . import cwhy

def main():
    text = asyncio.run(cwhy.complete(cwhy.cwhy_prompt()))
    print('\n'.join(textwrap.wrap(text, width=70)))

