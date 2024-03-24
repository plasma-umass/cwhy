# file src/cwhy/conversation/diff_functions.py:121-125
# lines [121, 122, 123, 124]
# branches []

import pytest
from unittest.mock import MagicMock
from cwhy.conversation.diff_functions import DiffFunctions

def test_try_compiling_schema():
    mock_args = MagicMock()
    diff_functions = DiffFunctions(mock_args)
    schema = diff_functions.try_compiling_schema()
    assert schema == {
        "name": "try_compiling",
        "description": "Attempts to compile the code again after the user has made changes. Returns the new error message if there is one.",
    }
