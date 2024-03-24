# file src/cwhy/cwhy.py:143-159
# lines [144, 145, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 159]
# branches ['144->145', '144->147', '147->148', '147->149', '149->150', '149->154', '154->155', '154->156', '156->157', '156->159']

import pytest
from src.cwhy.cwhy import evaluate
from unittest.mock import MagicMock, patch

# Define a test function to cover lines 144-159
@pytest.fixture
def mock_evaluate_with_fallback():
    with patch('src.cwhy.cwhy.evaluate_with_fallback') as mock:
        yield mock

@pytest.fixture
def mock_evaluate_text_prompt():
    with patch('src.cwhy.cwhy.evaluate_text_prompt') as mock:
        yield mock

@pytest.fixture
def mock_evaluate_diff():
    with patch('src.cwhy.cwhy.evaluate_diff') as mock:
        yield mock

@pytest.fixture
def mock_converse():
    with patch('src.cwhy.cwhy.conversation.converse') as mock:
        yield mock

@pytest.fixture
def mock_diff_converse():
    with patch('src.cwhy.cwhy.conversation.diff_converse') as mock:
        yield mock

@pytest.fixture
def mock_prompts_explain_prompt():
    with patch('src.cwhy.cwhy.prompts.explain_prompt') as mock:
        yield mock

def test_evaluate_coverage(mock_evaluate_with_fallback, mock_evaluate_text_prompt, mock_evaluate_diff, mock_converse, mock_diff_converse, mock_prompts_explain_prompt):
    # Create a mock args object with the necessary attributes
    mock_args = MagicMock()
    mock_stdin = MagicMock()

    # Test the "default" llm branch
    mock_args.llm = "default"
    evaluate(mock_args, mock_stdin)
    mock_evaluate_with_fallback.assert_called_once_with(mock_args, mock_stdin)

    # Test the "explain" subcommand branch
    mock_args.llm = "non-default"
    mock_args.subcommand = "explain"
    evaluate(mock_args, mock_stdin)
    mock_prompts_explain_prompt.assert_called_once_with(mock_args, mock_stdin)
    mock_evaluate_text_prompt.assert_called_once_with(mock_args, mock_prompts_explain_prompt.return_value)

    # Test the "diff" subcommand branch
    mock_args.subcommand = "diff"
    mock_evaluate_diff.return_value.choices = [MagicMock(message=MagicMock(tool_calls=[MagicMock(function=MagicMock(arguments='diff-args'))]))]
    result = evaluate(mock_args, mock_stdin)
    assert result == 'diff-args'

    # Test the "converse" subcommand branch
    mock_args.subcommand = "converse"
    evaluate(mock_args, mock_stdin)
    mock_converse.assert_called_once_with(mock_args, mock_stdin)

    # Test the "diff-converse" subcommand branch
    mock_args.subcommand = "diff-converse"
    evaluate(mock_args, mock_stdin)
    mock_diff_converse.assert_called_once_with(mock_args, mock_stdin)

    # Test the unknown subcommand branch
    mock_args.subcommand = "unknown"
    with pytest.raises(Exception) as exc_info:
        evaluate(mock_args, mock_stdin)
    assert str(exc_info.value) == f"unknown subcommand: {mock_args.subcommand}"
