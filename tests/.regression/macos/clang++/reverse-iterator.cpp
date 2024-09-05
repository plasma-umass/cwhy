===================== Prompt =====================
This is my code:

File `/Users/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp`:
```
25 */
26 #include <algorithm>
27 #include <string>
28 
29 std::string trim(std::string s) {
30     static const auto isNotSpace = [](auto c) { return !std::isspace(c); };
31     s.erase(s.begin(), std::find_if(s.begin(), s.end(), isNotSpace));
32     s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
33     return s;
34 }
```

File `/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/string`:
```
1442 #ifndef _LIBCPP_CXX03_LANG
1443     _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
1444     iterator insert(const_iterator __pos, initializer_list<value_type> __il)
1445                     {return insert(__pos, __il.begin(), __il.end());}
1446 #endif // _LIBCPP_CXX03_LANG
1447 
1448     _LIBCPP_CONSTEXPR_SINCE_CXX20 basic_string& erase(size_type __pos = 0, size_type __n = npos);
1449     _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
1450     iterator      erase(const_iterator __pos);
1451     _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
1452     iterator      erase(const_iterator __first, const_iterator __last);
1453 
1454   _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 basic_string&
1455   replace(size_type __pos1, size_type __n1, const basic_string& __str) {
```


This is my error:
```
/Users/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:32:7: error: no matching member function for call to 'erase'
    s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
    ~~^~~~~
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/string:1452:19: note: candidate function not viable: no known conversion from 'std::reverse_iterator<std::__wrap_iter<char *>>' to 'const_iterator' (aka '__wrap_iter<const char *>') for 1st argument
    iterator      erase(const_iterator __first, const_iterator __last);
                  ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/string:1448:49: note: candidate function not viable: no known conversion from 'std::reverse_iterator<std::__wrap_iter<char *>>' to 'size_type' (aka 'unsigned long') for 1st argument
    _LIBCPP_CONSTEXPR_SINCE_CXX20 basic_string& erase(size_type __pos = 0, size_type __n = npos);
                                                ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/string:1450:19: note: candidate function not viable: requires single argument '__pos', but 2 arguments were provided
    iterator      erase(const_iterator __pos);
                  ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
