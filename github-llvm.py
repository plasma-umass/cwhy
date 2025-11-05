import math
import json
import os
import requests
import subprocess
import tempfile

BASE = "https://api.github.com"
TOKEN = os.getenv("GITHUB_PAT", None)
HEADERS = {
    "Accept": "application/vnd.github+json",
    "X-GitHub-Api-Version": "2022-11-28",
    **({"Authorization": f"Bearer {TOKEN}"} if TOKEN else {}),
}


class LogGroup:
    def __init__(self, name: str | None = None):
        self.name = name
        self.content: list[str | LogGroup] = []

    def append_line(self, line: str) -> None:
        self.content.append(line.strip())

    def append_child(self, group: "LogGroup") -> None:
        self.content.append(group)

    def __str__(self) -> str:
        def _str_helper(group: LogGroup, indent: int) -> str:
            result = ""
            indent_str = "    " * indent
            result += f"{indent_str}Group: {group.name or '[unnamed]'}\n"
            for item in group.content:
                if isinstance(item, LogGroup):
                    result += _str_helper(item, indent + 1)
                else:
                    result += f"{indent_str}    {item}\n"
            return result

        return _str_helper(self, 0)

    def get_child_group(self, name: str) -> "LogGroup | None":
        for item in self.content:
            if isinstance(item, LogGroup) and item.name == name:
                return item
        return None

    def __iter__(self):
        for item in self.content:
            if isinstance(item, LogGroup):
                yield from item
            else:
                yield item


def group_job_log(log: str) -> LogGroup:
    def _recursive_helper(lines: list[str], index: int, group: LogGroup) -> int:
        while index < len(lines):
            line = lines[index]
            line = line[line.find(" ") + 1 :]  # Remove timestamp.
            if line.startswith("##[group]"):
                child = LogGroup(line[len("##[group]") :].strip() or None)
                index = _recursive_helper(lines, index + 1, child)
                group.append_child(child)
            elif line.startswith("##[endgroup]"):
                return index + 1
            else:
                group.append_line(line)
                index += 1
        return index

    root = LogGroup()
    _recursive_helper(log.splitlines(), 0, root)
    return root


# https://docs.github.com/en/rest/actions/workflow-jobs?apiVersion=2022-11-28#download-job-logs-for-a-workflow-run
def get_workflow_job_logs(repo: str, job_id: int) -> str:
    response = requests.get(
        f"{BASE}/repos/{repo}/actions/jobs/{job_id}/logs", headers=HEADERS
    )
    return response.text


# https://docs.github.com/en/rest/actions/workflow-jobs?apiVersion=2022-11-28#list-jobs-for-a-workflow-run
def get_workflow_jobs(repo: str, run_id: int):
    response = requests.get(
        f"{BASE}/repos/{repo}/actions/runs/{run_id}/jobs",
        headers=HEADERS,
        params={"per_page": 100, "filter": "latest"},
    )
    assert "link" not in response.headers  # No support for pagination.
    return response.json()["jobs"]


# https://docs.github.com/en/rest/actions/workflow-runs?apiVersion=2022-11-28#list-workflow-runs-for-a-repository
def get_workflow_runs(
    repo: str,
    event: str | None = None,
    status: str | None = None,
    pagination: int = 100,
):
    total_count = math.inf  # 2025-10: Max we can get is 1000 entries anyway.
    i = 0
    while i < total_count:
        response = requests.get(
            f"{BASE}/repos/{repo}/actions/runs",
            headers=HEADERS,
            params={
                **({"event": event} if event else {}),
                **({"status": status} if status else {}),
                "per_page": pagination,
                "page": i // pagination + 1,
            },
        )
        data = response.json()
        total_count = data["total_count"]
        i += len(data["workflow_runs"])
        for run in data["workflow_runs"]:
            yield run


# We now want to clone the original repo, add the other repo as a remote.
# Then fetch the commit, and find the merge base.

# 1. git clone https://github.com/llvm/llvm-project
# 2. git rev-parse --verify HEAD
# 3. git remote add XXX/llvm-project https://github.com/XXX/llvm-project.git
# 4. git fetch XXX/llvm-project YYY
# 5. git merge-base HEAD YYY
# 6. git diff MERGE_BASE..YYY


def get_bases_and_diffs(
    repo: str, head_repos_and_shas: list[tuple[str, str]]
) -> list[tuple[str, str]]:
    bases_and_diffs = []

    with tempfile.TemporaryDirectory() as directory:
        subprocess.run(
            ["git", "clone", f"https://github.com/{repo}.git", directory],
            check=True,
        )
        head = subprocess.check_output(
            ["git", "rev-parse", "--verify", "HEAD"],
            cwd=directory,
            text=True,
        ).strip()

        for head_repo, head_sha in head_repos_and_shas:
            remotes = [
                line.split()[0]
                for line in subprocess.check_output(
                    ["git", "remote", "-v"],
                    cwd=directory,
                    text=True,
                )
                .strip()
                .splitlines()
            ]
            if head_repo not in remotes:
                subprocess.run(
                    [
                        "git",
                        "remote",
                        "add",
                        head_repo,
                        f"https://github.com/{head_repo}.git",
                    ],
                    cwd=directory,
                    check=True,
                )

            fetch_process = subprocess.run(
                ["git", "fetch", head_repo, head_sha], cwd=directory
            )
            if fetch_process.returncode != 0:
                # Commit or repository deleted.
                continue

            merge_base = subprocess.check_output(
                ["git", "merge-base", head, head_sha],
                cwd=directory,
                text=True,
            ).strip()

            diff = subprocess.check_output(
                ["git", "diff", f"{merge_base}..{head_sha}"],
                cwd=directory,
                text=True,
            )

            bases_and_diffs.append((merge_base, diff))
    return bases_and_diffs


# Workflow runs can be "failed" with no "failed" jobs, e.g. when workflow file is invalid.
# Jobs can be "failed" with no "failed" steps, e.g. when runner crashes.

entries = []

for i, run in enumerate(
    get_workflow_runs("llvm/llvm-project", event="pull_request", status="failure")
):
    if run["path"] != ".github/workflows/premerge.yaml":
        continue
    jobs = get_workflow_jobs("llvm/llvm-project", run["id"])
    for job in jobs:
        if job["name"] != "Build and Test Linux":
            continue
        if job["conclusion"] != "failure":
            continue

        logs = get_workflow_job_logs("llvm/llvm-project", job["id"])

        log = group_job_log(logs).get_child_group("CMake")
        if not log:
            continue
        log = log.get_child_group("ninja")
        if not log:
            continue

        log = str(log)
        # Find a line with "] Building CXX" followed by a line with "FAILED: ".
        lines = log.splitlines()
        for j in range(len(lines) - 1):
            if "] Building CXX" in lines[j] and "FAILED: " in lines[j + 1]:
                print("Run:", i)
                print(f"Repository: {run['repository']['full_name']}")
                print(f"Head repository: {run['head_repository']['full_name']}")
                print("Commit:", run["head_sha"])
                print("URL:", job["html_url"])
                print()

                entries.append(
                    (
                        run["head_repository"]["full_name"],
                        run["head_sha"],
                    )
                )
                break

data = get_bases_and_diffs("llvm/llvm-project", entries)
print("Retrieved", len(entries), "failed CI commits...")
print("Retrieved", len(data), "actual commits...")
with open("commits.json", "w") as f:
    json.dump(data, f)
