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

File `/usr/include/c++/12/bits/basic_string.h`:
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

File `/usr/include/c++/12/bits/basic_string.h`:
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

File `/usr/include/c++/12/bits/basic_string.h`:
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
/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp: In function ‘std::string trim(std::string)’:
/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:32:12: error: no matching function for call to ‘std::__cxx11::basic_string<char>::erase(std::reverse_iterator<__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> > >, std::__cxx11::basic_string<char>::iterator)’
   32 |     s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
      |     ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /usr/include/c++/12/string:53,
                 from /home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:27:
/usr/include/c++/12/bits/basic_string.h:2040:7: note: candidate: ‘constexpr std::__cxx11::basic_string<_CharT, _Traits, _Alloc>& std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::erase(size_type, size_type) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>; size_type = long unsigned int]’
 2040 |       erase(size_type __pos = 0, size_type __n = npos)
      |       ^~~~~
/usr/include/c++/12/bits/basic_string.h:2040:23: note:   no known conversion for argument 1 from ‘std::reverse_iterator<__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> > >’ to ‘std::__cxx11::basic_string<char>::size_type’ {aka ‘long unsigned int’}
 2040 |       erase(size_type __pos = 0, size_type __n = npos)

[...]

      |             ~~~~~~~~~~^~~~~~~~~
/usr/include/c++/12/bits/basic_string.h:2060:7: note: candidate: ‘constexpr std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::iterator std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::erase(__const_iterator) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>; iterator = std::__cxx11::basic_string<char>::iterator; __const_iterator = std::__cxx11::basic_string<char>::const_iterator]’
 2060 |       erase(__const_iterator __position)
      |       ^~~~~
/usr/include/c++/12/bits/basic_string.h:2060:7: note:   candidate expects 1 argument, 2 provided
/usr/include/c++/12/bits/basic_string.h:2080:7: note: candidate: ‘constexpr std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::iterator std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::erase(__const_iterator, __const_iterator) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>; iterator = std::__cxx11::basic_string<char>::iterator; __const_iterator = std::__cxx11::basic_string<char>::const_iterator]’
 2080 |       erase(__const_iterator __first, __const_iterator __last)
      |       ^~~~~
/usr/include/c++/12/bits/basic_string.h:2080:30: note:   no known conversion for argument 1 from ‘std::reverse_iterator<__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> > >’ to ‘std::__cxx11::basic_string<char>::__const_iterator’ {aka ‘std::__cxx11::basic_string<char>::const_iterator’}
 2080 |       erase(__const_iterator __first, __const_iterator __last)
      |             ~~~~~~~~~~~~~~~~~^~~~~~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
