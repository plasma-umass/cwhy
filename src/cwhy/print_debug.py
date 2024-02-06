import inspect

_debug = False

_INFO_WIDTH = 30


def dprint(*objects, sep=" ", end="\n", file=None, flush=False):
    if not _debug:
        return print(*objects, sep=sep, end=end, file=file, flush=flush)

    frame = inspect.currentframe()
    caller = inspect.getouterframes(frame, 2)
    filename = caller[1].filename
    lineno = caller[1].lineno

    info = f"{filename}:{lineno}"
    if len(info) > _INFO_WIDTH:
        info = f"...{info[-_INFO_WIDTH + 3:]}"
    info = info.ljust(_INFO_WIDTH - 2)
    info = f"{info} |"

    message = sep.join(map(str, objects)) + end
    for line in message.splitlines():
        print(info, line, file=file, flush=flush)


def enable_debug_printing():
    global _debug
    _debug = True
