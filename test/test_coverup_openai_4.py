# file src/cwhy/__main__.py:39-48
# lines [39, 40, 41, 42, 44, 46, 47]
# branches []

import os
import pytest
from cwhy.__main__ import win_wrapper

def test_win_wrapper(tmp_path):
    python_filename = "test_script.py"
    wrapper_content = win_wrapper(python_filename)

    # Create a temporary directory to hold the wrapper
    temp_dir = tmp_path / "test_wrapper"
    temp_dir.mkdir()
    wrapper_path = temp_dir / "wrapper.bat"

    # Write the wrapper content to a .bat file
    with open(wrapper_path, "w") as f:
        f.write(wrapper_content)

    # Assert that the wrapper content is correct
    expected_content = "@echo off\npython test_script.py %*\n"
    assert wrapper_content == expected_content

    # Clean up the created files
    wrapper_path.unlink()
    temp_dir.rmdir()
