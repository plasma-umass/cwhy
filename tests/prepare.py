import os
import shutil
import subprocess

import yaml

ROOT = os.path.dirname(os.path.abspath(__file__))

def get_flags(language, benchmark):
    try:
        with open(os.path.join(ROOT, language, "manifest.yml"), "r") as stream:
            manifest = yaml.load(stream, yaml.Loader)
        flags = manifest.get(benchmark, {}).get("flags", {})
        for flag, value in flags.items():
            flags[flag] = value.format(
                DEPENDENCIES_SOURCE=os.path.join(ROOT, language, "_deps", "src"),
                DEPENDENCIES_BUILD=os.path.join(ROOT, language, "_deps", "build"),
                DEPENDENCIES_INSTALL=os.path.join(ROOT, language, "_deps", "install"),
            )
        return flags
    except FileNotFoundError:
        return {}


def prepare(language):
    try:
        with open(os.path.join(ROOT, language, "manifest.yml"), "r") as stream:
            manifest = yaml.load(stream, yaml.Loader)
    except FileNotFoundError:
        return

    for test in manifest.keys():
        for command in manifest[test]["prepare"]:
            command = command.format(
                DEPENDENCIES_SOURCE=os.path.join(ROOT, language, "_deps", "src"),
                DEPENDENCIES_BUILD=os.path.join(ROOT, language, "_deps", "build"),
                DEPENDENCIES_INSTALL=os.path.join(ROOT, language, "_deps", "install"),
            )
            subprocess.run(command, shell=True).check_returncode()


def prepare_all():
    for directory in os.listdir(ROOT):
        if os.path.isdir(os.path.join(ROOT, directory)):
            prepare(directory)


def clean():
    for directory in os.listdir(ROOT):
        if os.path.isdir(os.path.join(ROOT, directory)):
            shutil.rmtree(os.path.join(ROOT, directory, "_deps"), ignore_errors=True)
