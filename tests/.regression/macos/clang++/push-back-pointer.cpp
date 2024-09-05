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

File `/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/vector`:
```
642     value_type*       data() _NOEXCEPT
643         {return std::__to_address(this->__begin_);}
644 
645     _LIBCPP_CONSTEXPR_SINCE_CXX20 _LIBCPP_HIDE_FROM_ABI
646     const value_type* data() const _NOEXCEPT
647         {return std::__to_address(this->__begin_);}
648 
649     _LIBCPP_CONSTEXPR_SINCE_CXX20 _LIBCPP_HIDE_FROM_ABI void push_back(const_reference __x);
650 
651     _LIBCPP_CONSTEXPR_SINCE_CXX20 _LIBCPP_HIDE_FROM_ABI void push_back(value_type&& __x);
652 
653     template <class... _Args>
654     _LIBCPP_CONSTEXPR_SINCE_CXX20 _LIBCPP_HIDE_FROM_ABI
```


This is my error:
```
/Users/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:7: error: no matching member function for call to 'push_back'
    v.push_back(pointer);
    ~~^~~~~~~~~
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/vector:649:62: note: candidate function not viable: no known conversion from 'int *' to 'const value_type' (aka 'const int') for 1st argument; dereference the argument with *
    _LIBCPP_CONSTEXPR_SINCE_CXX20 _LIBCPP_HIDE_FROM_ABI void push_back(const_reference __x);
                                                             ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/vector:651:62: note: candidate function not viable: no known conversion from 'int *' to 'value_type' (aka 'int') for 1st argument; dereference the argument with *
    _LIBCPP_CONSTEXPR_SINCE_CXX20 _LIBCPP_HIDE_FROM_ABI void push_back(value_type&& __x);
                                                             ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
