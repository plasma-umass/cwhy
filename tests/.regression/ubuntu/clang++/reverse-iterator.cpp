===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp`:
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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h`:
```
2050        *  there are < @a __n characters to remove, the remainder of
2051        *  the string is truncated.  If @a __p is beyond end of string,
2052        *  out_of_range is thrown.  The value of the string doesn't
2053        *  change if an error is thrown.
2054       */
2055       _GLIBCXX20_CONSTEXPR
2056       basic_string&
2057       erase(size_type __pos = 0, size_type __n = npos)
2058       {
2059 	_M_check(__pos, "basic_string::erase");
2060 	if (__n == npos)
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h`:
```
2071        *
2072        *  Removes the character at @a __position from this string. The value
2073        *  of the string doesn't change if an error is thrown.
2074       */
2075       _GLIBCXX20_CONSTEXPR
2076       iterator
2077       erase(__const_iterator __position)
2078       {
2079 	_GLIBCXX_DEBUG_PEDASSERT(__position >= begin()
2080 				 && __position < end());
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h`:
```
2091        *
2092        *  Removes the characters in the range [first,last) from this string.
2093        *  The value of the string doesn't change if an error is thrown.
2094       */
2095       _GLIBCXX20_CONSTEXPR
2096       iterator
2097       erase(__const_iterator __first, __const_iterator __last)
2098       {
2099 	_GLIBCXX_DEBUG_PEDASSERT(__first >= begin() && __first <= __last
2100 				 && __last <= end());
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:32:7: error: no matching member function for call to 'erase'
   32 |     s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
      |     ~~^~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h:2097:7: note: candidate function not viable: no known conversion from 'std::reverse_iterator<__gnu_cxx::__normal_iterator<char *, std::basic_string<char>>>' to '__const_iterator' (aka '__normal_iterator<const char *, std::basic_string<char, std::char_traits<char>, std::allocator<char>>>') for 1st argument
 2097 |       erase(__const_iterator __first, __const_iterator __last)
      |       ^     ~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h:2057:7: note: candidate function not viable: no known conversion from 'std::reverse_iterator<__gnu_cxx::__normal_iterator<char *, std::basic_string<char>>>' to 'size_type' (aka 'unsigned long') for 1st argument

[...]

 2057 |       erase(size_type __pos = 0, size_type __n = npos)
      |       ^     ~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h:2077:7: note: candidate function not viable: requires single argument '__position', but 2 arguments were provided
 2077 |       erase(__const_iterator __position)
      |       ^     ~~~~~~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
