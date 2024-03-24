# file src/cwhy/conversation/utils.py:7-34
# lines [7, 11, 12, 13, 14, 16, 17, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34]
# branches ['19->20', '19->32', '20->21', '20->24', '28->19', '28->29', '32->33', '32->34']

import argparse
from unittest.mock import patch
import pytest

# Assuming llm_utils.count_tokens is a function that needs to be mocked
from cwhy.conversation import llm_utils
from cwhy.conversation.utils import get_truncated_error_message

@pytest.fixture
def mock_count_tokens():
    with patch('cwhy.conversation.llm_utils.count_tokens') as mock:
        mock.side_effect = lambda llm, text: len(text.split())
        yield mock

def test_get_truncated_error_message(mock_count_tokens):
    args = argparse.Namespace()
    args.llm = "mock_llm"
    args.max_error_tokens = 10  # Set a max token limit for the test

    # Create a diagnostic message that will exceed the max_error_tokens when combined
    diagnostic = "Error on line 1\nError on line 2\nError on line 3\nError on line 4"

    truncated_message = get_truncated_error_message(args, diagnostic)

    # Check that the message was indeed truncated
    assert "[...]" in truncated_message

    # Check that the truncated message does not exceed max_error_tokens
    assert len(truncated_message.split()) <= args.max_error_tokens

    # Check that the first and last lines are present in the truncated message
    assert "Error on line 1" in truncated_message
    assert "Error on line 4" in truncated_message

    # Check that the middle lines are not present in the truncated message
    assert "Error on line 2" not in truncated_message
    assert "Error on line 3" not in truncated_message
