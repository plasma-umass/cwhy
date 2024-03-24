# file src/cwhy/conversation/explain_functions.py:54-72
# lines [54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 65, 66, 67, 70]
# branches []

import pytest
from unittest.mock import MagicMock
from src.cwhy.conversation.explain_functions import ExplainFunctions

def test_get_code_surrounding_schema():
    mock_args = MagicMock()
    explain_functions = ExplainFunctions(mock_args)
    schema = explain_functions.get_code_surrounding_schema()
    assert schema == {
        "name": "get_code_surrounding",
        "description": "Returns the code in the given file surrounding and including the provided line number.",
        "parameters": {
            "type": "object",
            "properties": {
                "filename": {
                    "type": "string",
                    "description": "The filename to read from.",
                },
                "lineno": {
                    "type": "integer",
                    "description": "The line number to focus on. Some context before and after that line will be provided.",
                },
            },
            "required": ["filename", "lineno"],
        },
    }
