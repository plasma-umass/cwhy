# file src/cwhy/cwhy.py:26-43
# lines [27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42]
# branches []

import pytest
from unittest.mock import patch, call
from src.cwhy.cwhy import print_key_info

def test_print_key_info():
    with patch('src.cwhy.cwhy.dprint') as mock_dprint:
        print_key_info()
        assert mock_dprint.call_count == 15
        calls = [
            call("You need a key (or keys) from an AI service to use CWhy."),
            call(),
            call("OpenAI:"),
            call("  You can get a key here: https://platform.openai.com/api-keys"),
            call("  Set the environment variable OPENAI_API_KEY to your key value:"),
            call("    export OPENAI_API_KEY=<your key>"),
            call(),
            call("Bedrock:"),
            call("  To use Bedrock, you need an AWS account."),
            call("  Set the following environment variables:"),
            call("    export AWS_ACCESS_KEY_ID=<your key id>"),
            call("    export AWS_SECRET_ACCESS_KEY=<your secret key>"),
            call("    export AWS_REGION_NAME=us-west-2"),
            call("  You also need to request access to Claude:"),
            call(
                "   https://docs.aws.amazon.com/bedrock/latest/userguide/model-access.html#manage-model-access"
            ),
        ]
        mock_dprint.assert_has_calls(calls, any_order=True)
