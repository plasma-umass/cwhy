# file src/cwhy/print_debug.py:28-30
# lines [30]
# branches []

import pytest
from cwhy.print_debug import enable_debug_printing

# Assuming that _debug is a global variable within the cwhy.print_debug module
# and that there is no direct way to check if _debug is True other than calling
# enable_debug_printing and observing its effects.

def test_enable_debug_printing(monkeypatch):
    # Use monkeypatch to set _debug to False before the test
    monkeypatch.setattr('cwhy.print_debug._debug', False)
    
    # Call the function that should set _debug to True
    enable_debug_printing()
    
    # Import _debug to check its value after enable_debug_printing() is called
    from cwhy.print_debug import _debug
    assert _debug == True
