# file src/cwhy/prompts.py:194-198
# lines [194, 195, 196, 197]
# branches []

import argparse
from cwhy.prompts import explain_prompt
import pytest
from unittest.mock import patch

# Assuming the _base_prompt function is something like this:
# def _base_prompt(args: argparse.Namespace, diagnostic: str) -> str:
#     return f"Diagnostic: {diagnostic}\n"

# Test function to cover explain_prompt
def test_explain_prompt():
    # Mock the _base_prompt function to control its output
    with patch('cwhy.prompts._base_prompt', return_value="Mocked base prompt. ") as mock_base_prompt:
        # Create a mock argparse.Namespace object
        mock_args = argparse.Namespace()
        diagnostic = "Sample diagnostic message."

        # Call the function under test
        result = explain_prompt(mock_args, diagnostic)

        # Verify the result includes the mocked base prompt and the additional text
        expected_result = "Mocked base prompt. What's the problem? If you can, suggest code to fix the issue."
        assert result == expected_result
