# file src/cwhy/conversation/diff_functions.py:48-79
# lines [48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 60, 61, 63, 64, 65, 67, 68, 69, 72]
# branches []

import pytest
from cwhy.conversation.diff_functions import DiffFunctions

class MockArgs:
    pass

def test_apply_modification_schema():
    mock_args = MockArgs()
    diff_functions = DiffFunctions(mock_args)
    schema = diff_functions.apply_modification_schema()
    assert schema['name'] == 'apply_modification'
    assert schema['description'] == 'Applies a single modification to the source file with the goal of fixing any existing compilation errors.'
    assert 'parameters' in schema
    params = schema['parameters']
    assert params['type'] == 'object'
    assert 'properties' in params
    properties = params['properties']
    assert 'filename' in properties
    assert properties['filename']['type'] == 'string'
    assert 'start-line-number' in properties
    assert properties['start-line-number']['type'] == 'integer'
    assert 'number-lines-remove' in properties
    assert properties['number-lines-remove']['type'] == 'integer'
    assert 'replacement' in properties
    assert properties['replacement']['type'] == 'string'
    assert 'required' in params
    required_fields = params['required']
    assert 'filename' in required_fields
    assert 'start-line-number' in required_fields
    assert 'number-lines-remove' in required_fields
    assert 'replacement' in required_fields
