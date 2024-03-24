# file src/cwhy/prompts.py:178-191
# lines [178, 179, 181, 182, 183, 184, 185, 186, 187, 188, 189, 191]
# branches ['183->184', '183->187']

import argparse
import pytest
from unittest.mock import patch
from cwhy.prompts import _base_prompt

class MockContext:
    def __init__(self, args, diagnostic):
        self.args = args
        self.diagnostic = diagnostic

    def get_code(self):
        return self.args.code

    def get_diagnostic(self):
        return self.diagnostic

@pytest.fixture
def mock_context(monkeypatch):
    def mock_init(self, args, diagnostic):
        self.args = args
        self.diagnostic = diagnostic

    monkeypatch.setattr(MockContext, "__init__", mock_init)
    monkeypatch.setattr('cwhy.prompts._Context', MockContext)

def test_base_prompt_with_code(mock_context):
    args = argparse.Namespace(code="print('Hello, World!')")
    diagnostic = "NameError: name 'Hello' is not defined"
    expected_prompt = (
        "This is my code:\n\n"
        "print('Hello, World!')\n"
        "This is my error:\n"
        "NameError: name 'Hello' is not defined\n\n"
    )
    prompt = _base_prompt(args, diagnostic)
    assert prompt == expected_prompt

def test_base_prompt_without_code(mock_context):
    args = argparse.Namespace(code=None)
    diagnostic = "SyntaxError: invalid syntax"
    expected_prompt = (
        "This is my error:\n"
        "SyntaxError: invalid syntax\n\n"
    )
    prompt = _base_prompt(args, diagnostic)
    assert prompt == expected_prompt
