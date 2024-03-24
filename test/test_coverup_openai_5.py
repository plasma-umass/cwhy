# file src/cwhy/conversation/explain_functions.py:43-47
# lines [43, 44, 45, 46]
# branches []

import pytest
from unittest.mock import MagicMock
from src.cwhy.conversation.explain_functions import ExplainFunctions

def test_get_compile_or_run_command_schema():
    mock_args = MagicMock()
    explain_functions = ExplainFunctions(mock_args)
    schema = explain_functions.get_compile_or_run_command_schema()
    assert schema == {
        "name": "get_compile_or_run_command",
        "description": "Returns the command used to compile or run the code. This will include any flags and options used.",
    }
