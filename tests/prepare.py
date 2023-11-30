import os
import shutil
import subprocess

import yaml

ROOT = os.path.dirname(os.path.abspath(__file__))


def prepare(language):
    try:
        with open(os.path.join(ROOT, language, "manifest.yml"), "r") as stream:
            manifest = yaml.load(stream, yaml.Loader)
    except FileNotFoundError:
        return

    for test in manifest.keys():
        for command in manifest[test]["prepare"]:
            command = command.format(
                DEPENDENCIES_SOURCES=os.path.join(ROOT, "_deps", language, test, "src"),
                DEPENDENCIES_BUILD=os.path.join(ROOT, "_deps", language, test, "build"),
                DEPENDENCIES_INSTALL=os.path.join(
                    ROOT, "_deps", language, test, "install"
                ),
            )
            subprocess.run(command, shell=True).check_returncode()


def prepare_all():
    for directory in os.listdir(ROOT):
        if os.path.isdir(os.path.join(ROOT, directory)):
            prepare(directory)


def clean():
    shutil.rmtree(os.path.join(ROOT, "_deps"), ignore_errors=True)
