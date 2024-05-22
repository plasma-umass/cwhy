#! /usr/bin/env python3

import argparse
import tempfile
from typing import Optional, Tuple


def anonymize(path: str) -> Tuple[str, Optional[str]]:
    with open(path) as original_file:
        content = original_file.read()

        code = content
        answer = None

        try:
            i = content.index("/*")
            j = content.index("*/", i)

            code = content[j + 2 :].strip()
            answer = content[i + 2 : j].strip()
        except ValueError:
            pass

    temporary_file = tempfile.NamedTemporaryFile(delete=False)
    temporary_file.write(code.encode("utf-8"))
    temporary_file.close()

    return temporary_file.name, answer


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("path", type=str, help="the path to the file to anonymize")
    args = parser.parse_args()
    filename, _ = anonymize(args.path)
    print(filename)
