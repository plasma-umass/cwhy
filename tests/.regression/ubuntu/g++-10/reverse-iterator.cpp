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

File `/usr/include/c++/10/bits/basic_string.h`:
```
1812        *  __pos.  The length of the string is reduced by @a __n.  If
1813        *  there are < @a __n characters to remove, the remainder of
1814        *  the string is truncated.  If @a __p is beyond end of string,
1815        *  out_of_range is thrown.  The value of the string doesn't
1816        *  change if an error is thrown.
1817       */
1818       basic_string&
1819       erase(size_type __pos = 0, size_type __n = npos)
1820       {
1821 	_M_check(__pos, "basic_string::erase");
1822 	if (__n == npos)
```

File `/usr/include/c++/10/bits/basic_string.h`:
```
1832        *  @return  iterator referencing same location after removal.
1833        *
1834        *  Removes the character at @a __position from this string. The value
1835        *  of the string doesn't change if an error is thrown.
1836       */
1837       iterator
1838       erase(__const_iterator __position)
1839       {
1840 	_GLIBCXX_DEBUG_PEDASSERT(__position >= begin()
1841 				 && __position < end());
```

File `/usr/include/c++/10/bits/basic_string.h`:
```
1851        *  @return  Iterator referencing location of first after removal.
1852        *
1853        *  Removes the characters in the range [first,last) from this string.
1854        *  The value of the string doesn't change if an error is thrown.
1855       */
1856       iterator
1857       erase(__const_iterator __first, __const_iterator __last)
1858       {
1859 	_GLIBCXX_DEBUG_PEDASSERT(__first >= begin() && __first <= __last
1860 				 && __last <= end());
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp: In function ‘std::string trim(std::string)’:
/home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:32:68: error: no matching function for call to ‘std::__cxx11::basic_string<char>::erase(std::reverse_iterator<__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> > >, std::__cxx11::basic_string<char>::iterator)’
   32 |     s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
      |                                                                    ^
In file included from /usr/include/c++/10/string:55,
                 from /home/runner/work/cwhy/cwhy/tests/c++/reverse-iterator.cpp:27:
/usr/include/c++/10/bits/basic_string.h:1819:7: note: candidate: ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>& std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::erase(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::size_type, std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::size_type) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>; std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::size_type = long unsigned int]’
 1819 |       erase(size_type __pos = 0, size_type __n = npos)
      |       ^~~~~
/usr/include/c++/10/bits/basic_string.h:1819:23: note:   no known conversion for argument 1 from ‘std::reverse_iterator<__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> > >’ to ‘std::__cxx11::basic_string<char>::size_type’ {aka ‘long unsigned int’}
 1819 |       erase(size_type __pos = 0, size_type __n = npos)

[...]

      |             ~~~~~~~~~~^~~~~~~~~
/usr/include/c++/10/bits/basic_string.h:1838:7: note: candidate: ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::iterator std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::erase(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::__const_iterator) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>; std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::iterator = std::__cxx11::basic_string<char>::iterator; std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::__const_iterator = std::__cxx11::basic_string<char>::const_iterator]’
 1838 |       erase(__const_iterator __position)
      |       ^~~~~
/usr/include/c++/10/bits/basic_string.h:1838:7: note:   candidate expects 1 argument, 2 provided
/usr/include/c++/10/bits/basic_string.h:1857:7: note: candidate: ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::iterator std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::erase(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::__const_iterator, std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::__const_iterator) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>; std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::iterator = std::__cxx11::basic_string<char>::iterator; std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::__const_iterator = std::__cxx11::basic_string<char>::const_iterator]’
 1857 |       erase(__const_iterator __first, __const_iterator __last)
      |       ^~~~~
/usr/include/c++/10/bits/basic_string.h:1857:30: note:   no known conversion for argument 1 from ‘std::reverse_iterator<__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> > >’ to ‘std::__cxx11::basic_string<char>::__const_iterator’ {aka ‘std::__cxx11::basic_string<char>::const_iterator’}
 1857 |       erase(__const_iterator __first, __const_iterator __last)
      |             ~~~~~~~~~~~~~~~~~^~~~~~~
```


What's the problem?
==================================================
