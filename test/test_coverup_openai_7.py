# file src/cwhy/conversation/explain_functions.py:80-94
# lines [80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 92]
# branches []

import pytest
from unittest.mock import MagicMock
from src.cwhy.conversation.explain_functions import ExplainFunctions

def test_list_directory_schema():
    mock_args = MagicMock()
    explain_functions = ExplainFunctions(mock_args)
    schema = explain_functions.list_directory_schema()
    assert schema == {
        "name": "list_directory",
        "description": "Returns a list of all files and directories in the given directory.",
        "parameters": {
            "type": "object",
            "properties": {
                "path": {
                    "type": "string",
                    "description": "The path of the directory of interest.",
                },
            },
            "required": ["path"],
        },
    }
