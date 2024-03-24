# file src/cwhy/__main__.py:17-36
# lines [18, 19, 20, 21, 26, 30, 34, 35]
# branches []

import argparse
import os
import pytest
from unittest.mock import patch
from cwhy.__main__ import py_wrapper

@pytest.fixture
def mock_sys_argv():
    with patch('sys.argv', ['script_name', 'arg1', 'arg2']):
        yield

@pytest.fixture
def mock_os_environ():
    with patch.dict(os.environ, {}, clear=True):
        yield

@pytest.fixture
def mock_subprocess_run():
    with patch('subprocess.run') as mock:
        mock.return_value.returncode = 0
        yield mock

def test_py_wrapper_with_cwhy_disable(mock_sys_argv, mock_os_environ, mock_subprocess_run):
    os.environ['CWHY_DISABLE'] = '1'
    args = argparse.Namespace(command=['echo', 'Hello, World!'])
    script = py_wrapper(args)
    assert 'CWHY_DISABLE' in script
    assert 'subprocess.run' in script
    assert 'sys.exit' in script
    del os.environ['CWHY_DISABLE']

def test_py_wrapper_without_cwhy_disable(mock_sys_argv, mock_os_environ):
    args = argparse.Namespace(command=['echo', 'Hello, World!'])
    script = py_wrapper(args)
    assert 'argparse.Namespace' in script
    assert 'cwhy.main' in script
