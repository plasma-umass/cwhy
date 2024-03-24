# file src/cwhy/conversation/explain_functions.py:15-23
# lines [15, 16, 17, 18, 19, 20, 21]
# branches []

import pytest
from unittest.mock import MagicMock
from cwhy.conversation.explain_functions import ExplainFunctions

class TestExplainFunctions:
    def test_as_tools(self, monkeypatch):
        # Create a MagicMock to simulate the schema methods
        monkeypatch.setattr(ExplainFunctions, 'get_compile_or_run_command_schema', MagicMock(return_value={'name': 'compile_or_run'}))
        monkeypatch.setattr(ExplainFunctions, 'get_code_surrounding_schema', MagicMock(return_value={'name': 'code_surrounding'}))
        monkeypatch.setattr(ExplainFunctions, 'list_directory_schema', MagicMock(return_value={'name': 'list_directory'}))

        # Mock the __init__ method to not require arguments
        monkeypatch.setattr(ExplainFunctions, '__init__', lambda x, args: None)

        ef = ExplainFunctions(args=None)
        tools = ef.as_tools()

        # Verify that the list contains all three schemas
        assert len(tools) == 3
        assert tools[0]['function']['name'] == 'compile_or_run'
        assert tools[1]['function']['name'] == 'code_surrounding'
        assert tools[2]['function']['name'] == 'list_directory'
