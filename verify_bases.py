import argparse
import collections
import functools
import json
import subprocess
import tempfile
from typing import Callable


def is_ancestor_commit(path: str) -> Callable[[str, str], int]:
    def compare(a: str, b: str) -> int:
        returncode = subprocess.run(
            ["git", "merge-base", "--is-ancestor", a, b], cwd=path
        ).returncode
        return -1 if returncode == 0 else 1

    return compare


def main(args: argparse.Namespace) -> None:
    with open(args.json_file, "r") as f:
        data = json.load(f)
    with tempfile.TemporaryDirectory() as directory:
        print("Cloning repository...")
        subprocess.run(
            [
                "git",
                "clone",
                f"https://github.com/{args.repo}.git",
                "--branch=main",
                directory,
            ],
            check=True,
        )

        bases = list({base for base, _ in data})
        bases.sort(key=functools.cmp_to_key(is_ancestor_commit(directory)))

        diffs_by_base = collections.defaultdict(list)
        for base, diff in data:
            diffs_by_base[base].append(diff)

        for base in bases:
            print(f"Checking out {base}...")
            subprocess.run(
                ["git", "checkout", base],
                cwd=directory,
                check=True,
            )

            print("Running CMake...")
            subprocess.run(
                [
                    "cmake",
                    "llvm",
                    "-B",
                    "build",
                    "-G",
                    "Ninja",
                    # No debug info.
                    "-DCMAKE_BUILD_TYPE=Release",
                    "-DLLVM_ENABLE_PROJECTS='clang'",
                    # Max link jobs so we don't OOM.
                    "-DLLVM_PARALLEL_LINK_JOBS=4",
                ],
                cwd=directory,
                check=True,
            )

            print("Running Ninja for the base...")
            base_works = (
                subprocess.run(
                    # Only build Clang -- which includes pretty much all of LLVM.
                    ["ninja", "-C", "build", "clang"],
                    cwd=directory,
                ).returncode
                == 0
            )
            with open(args.log_file, "a") as log:
                print(base, base_works, file=log)

            if not base_works:
                continue

            print("Going through", len(diffs_by_base[base]), "diffs for this base...")
            for i, diff in enumerate(diffs_by_base[base]):
                with tempfile.NamedTemporaryFile("w", delete_on_close=False) as patch:
                    print(f"Applying patch #{i + 1} ({patch.name})...")
                    print(diff, file=patch)
                    patch.close()
                    subprocess.run(
                        [
                            "git",
                            "apply",
                            "--ignore-space-change",
                            "--ignore-whitespace",
                            patch.name,
                        ],
                        cwd=directory,
                        check=True,
                    )

                # Run ninja...
                print(f"Running ninja for patch #{i + 1}...")
                patch_works = (
                    subprocess.run(
                        # Only build Clang -- which includes pretty much all of LLVM.
                        ["ninja", "-C", "build", "clang"],
                        cwd=directory,
                    ).returncode
                    == 0
                )
                with open(args.log_file, "a") as log:
                    print("   ", i, patch_works, file=log)

                # Return to base.
                print("Returning to base changes...")
                subprocess.run(["git", "clean", "-xfd"], cwd=directory, check=True)
                subprocess.run(["git", "restore", "."], cwd=directory, check=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--json-file", type=str, default="commits.json")
    parser.add_argument("--log-file", type=str, default="commits.log")
    parser.add_argument("repo", type=str)
    main(parser.parse_args())
