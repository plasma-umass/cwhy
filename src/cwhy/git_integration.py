import os
import subprocess


def is_available() -> bool:
    try:
        return (
            subprocess.run(
                ["git", "status"],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            ).returncode
            == 0
        )
    except FileNotFoundError:
        return False


def get_git_status_porcelain() -> str:
    return subprocess.run(
        ["git", "status", "--porcelain"],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    ).stdout


def get_git_status_absolute_paths() -> list[str]:
    status = get_git_status_porcelain()
    paths = []
    for line in status.splitlines():
        path = line[3:].strip()
        path = os.path.abspath(path)
        paths.append(path)
    return paths


def get_git_diff(filename: str) -> str:
    return subprocess.run(
        ["git", "diff", filename],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    ).stdout.strip()
