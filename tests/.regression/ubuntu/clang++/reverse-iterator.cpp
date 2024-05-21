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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/basic_string.h`:
```
2033        *  there are < @a __n characters to remove, the remainder of
2034        *  the string is truncated.  If @a __p is beyond end of string,
2035        *  out_of_range is thrown.  The value of the string doesn't
2036        *  change if an error is thrown.
2037       */
2038       _GLIBCXX20_CONSTEXPR
2039       basic_string&
2040       erase(size_type __pos = 0, size_type __n = npos)
2041       {
2042 	_M_check(__pos, "basic_string::erase");
2043 	if (__n == npos)
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/basic_string.h`:
```
2054        *
2055        *  Removes the character at @a __position from this string. The value
2056        *  of the string doesn't change if an error is thrown.
2057       */
2058       _GLIBCXX20_CONSTEXPR
2059       iterator
2060       erase(__const_iterator __position)
2061       {
2062 	_GLIBCXX_DEBUG_PEDASSERT(__position >= begin()
2063 				 && __position < end());
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/basic_string.h`:
```
2074        *
2075        *  Removes the characters in the range [first,last) from this string.
2076        *  The value of the string doesn't change if an error is thrown.
2077       */
2078       _GLIBCXX20_CONSTEXPR
2079       iterator
2080       erase(__const_iterator __first, __const_iterator __last)
2081       {
2082 	_GLIBCXX_DEBUG_PEDASSERT(__first >= begin() && __first <= __last
2083 				 && __last <= end());
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:32:7: error: no matching member function for call to 'erase'
   32 |     s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
      |     ~~^~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/basic_string.h:2080:7: note: candidate function not viable: no known conversion from 'std::reverse_iterator<__gnu_cxx::__normal_iterator<char *, std::basic_string<char>>>' to '__const_iterator' (aka '__normal_iterator<const char *, std::basic_string<char, std::char_traits<char>, std::allocator<char>>>') for 1st argument
 2080 |       erase(__const_iterator __first, __const_iterator __last)
      |       ^     ~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/basic_string.h:2040:7: note: candidate function not viable: no known conversion from 'std::reverse_iterator<__gnu_cxx::__normal_iterator<char *, std::basic_string<char>>>' to 'size_type' (aka 'unsigned long') for 1st argument
 2040 |       erase(size_type __pos = 0, size_type __n = npos)
      |       ^     ~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/basic_string.h:2060:7: note: candidate function not viable: requires single argument '__position', but 2 arguments were provided
 2060 |       erase(__const_iterator __position)
      |       ^     ~~~~~~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
