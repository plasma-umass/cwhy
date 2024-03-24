# file src/cwhy/__main__.py:51-58
# lines [51, 52, 53, 54, 56, 57, 58]
# branches ['57->exit', '57->58']

import argparse
from unittest.mock import MagicMock
import pytest
from cwhy.__main__ import RichArgParser

def test_rich_arg_parser_print_message(monkeypatch):
    # Mock the Console object and its print method
    mock_console = MagicMock()
    monkeypatch.setattr('cwhy.__main__.Console', mock_console)

    # Create an instance of RichArgParser
    parser = RichArgParser()

    # Call the _print_message method with a test message
    test_message = "Test message"
    parser._print_message(test_message)

    # Assert that the console's print method was called with the test message
    mock_console.return_value.print.assert_called_once_with(test_message)

    # Call the _print_message method with None to test the branch where message is not printed
    parser._print_message(None)

    # Assert that the console's print method was still called only once (with the previous message)
    assert mock_console.return_value.print.call_count == 1
