#! /usr/bin/env python3

import argparse
import tempfile


def anonymize(path):
    with open(path) as file:
        content = file.read()

        code = content
        answer = None

        try:
            i = content.index("/*")
            j = content.index("*/", i)

            code = content[j + 2 :].strip()
            answer = content[i + 2 : j].strip()
        except ValueError:
            pass

    file = tempfile.NamedTemporaryFile(delete=False)
    file.write(code.encode("utf-8"))
    file.close()

    return file.name, answer


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("path", type=str, help="the path to the file to anonymize")
    args = parser.parse_args()
    filename, _ = anonymize(args.path)
    print(filename)
