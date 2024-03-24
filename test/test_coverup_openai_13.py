# file src/cwhy/conversation/explain_functions.py:49-52
# lines [49, 50, 51, 52]
# branches []

import pytest
from src.cwhy.conversation.explain_functions import ExplainFunctions
from unittest.mock import MagicMock, patch

@pytest.fixture
def explain_functions():
    mock_args = MagicMock()
    mock_args.command = ["python", "script.py"]
    return ExplainFunctions(args=mock_args)

def test_get_compile_or_run_command(explain_functions):
    with patch('src.cwhy.conversation.explain_functions.dprint') as mock_dprint:
        # Call the method and assert the result
        result = explain_functions.get_compile_or_run_command()
        assert result == "python script.py"

        # Assert that dprint was called with the correct argument
        mock_dprint.assert_called_once_with("python script.py")
