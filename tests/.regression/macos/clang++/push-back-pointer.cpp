===================== Prompt =====================
This is my code:

File `/Users/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp`:
```
17 */
18 #include <vector>
19 
20 int main() {
21     std::vector<int> v;
22     int value = 10;
23     int* pointer = &value;
24     v.push_back(pointer);
25 }
```

File `/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/vector`:
```
728     template <class _Arg>
729     _LIBCPP_INLINE_VISIBILITY
730     void __emplace_back(_Arg&& __arg) {
731       emplace_back(_VSTD::forward<_Arg>(__arg));
732     }
733 #endif
734 
735     _LIBCPP_INLINE_VISIBILITY void push_back(const_reference __x);
736 
737 #ifndef _LIBCPP_CXX03_LANG
738     _LIBCPP_INLINE_VISIBILITY void push_back(value_type&& __x);
739 
740     template <class... _Args>
741         _LIBCPP_INLINE_VISIBILITY
```


This is my error:
```
/Users/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:7: error: no matching member function for call to 'push_back'
    v.push_back(pointer);
    ~~^~~~~~~~~
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/vector:735:36: note: candidate function not viable: no known conversion from 'int *' to 'const std::__vector_base<int, std::allocator<int>>::value_type' (aka 'const int') for 1st argument; dereference the argument with *
    _LIBCPP_INLINE_VISIBILITY void push_back(const_reference __x);

[...]

                                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/vector:738:36: note: candidate function not viable: no known conversion from 'int *' to 'std::vector<int>::value_type' (aka 'int') for 1st argument; dereference the argument with *
    _LIBCPP_INLINE_VISIBILITY void push_back(value_type&& __x);
                                   ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
