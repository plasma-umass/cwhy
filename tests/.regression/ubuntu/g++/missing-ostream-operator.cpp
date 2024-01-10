===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp`:
```
23   private:
24     const std::string name;
25     const int age;
26 };
27 
28 int main() {
29     Cat cat("Scott", 13);
30     std::cout << cat << std::endl;
31 }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:15: error: no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘Cat’)
   30 |     std::cout << cat << std::endl;
      |     ~~~~~~~~~ ^~ ~~~
      |          |       |
      |          |       Cat
      |          std::ostream {aka std::basic_ostream<char>}
In file included from /usr/include/c++/12/iostream:39,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:18:
/usr/include/c++/12/ostream:108:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(__ostream_type& (*)(__ostream_type&)) [with _CharT = char; _Traits = std::char_traits<char>; __ostream_type = std::basic_ostream<char>]’
  108 |       operator<<(__ostream_type& (*__pf)(__ostream_type&))
      |       ^~~~~~~~
/usr/include/c++/12/ostream:108:36: note:   no known conversion for argument 1 from ‘Cat’ to ‘std::basic_ostream<char>::__ostream_type& (*)(std::basic_ostream<char>::__ostream_type&)’ {aka ‘std::basic_ostream<char>& (*)(std::basic_ostream<char>&)’}
  108 |       operator<<(__ostream_type& (*__pf)(__ostream_type&))
      |                  ~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/12/ostream:117:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(__ios_type& (*)(__ios_type&)) [with _CharT = char; _Traits = std::char_traits<char>; __ostream_type = std::basic_ostream<char>; __ios_type = std::basic_ios<char>]’
  117 |       operator<<(__ios_type& (*__pf)(__ios_type&))
      |       ^~~~~~~~
/usr/include/c++/12/ostream:117:32: note:   no known conversion for argument 1 from ‘Cat’ to ‘std::basic_ostream<char>::__ios_type& (*)(std::basic_ostream<char>::__ios_type&)’ {aka ‘std::basic_ios<char>& (*)(std::basic_ios<char>&)’}
  117 |       operator<<(__ios_type& (*__pf)(__ios_type&))
      |                  ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~
/usr/include/c++/12/ostream:127:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(std::ios_base& (*)(std::ios_base&)) [with _CharT = char; _Traits = std::char_traits<char>; __ostream_type = std::basic_ostream<char>]’
  127 |       operator<<(ios_base& (*__pf) (ios_base&))
      |       ^~~~~~~~
/usr/include/c++/12/ostream:127:30: note:   no known conversion for argument 1 from ‘Cat’ to ‘std::ios_base& (*)(std::ios_base&)’
  127 |       operator<<(ios_base& (*__pf) (ios_base&))
      |                  ~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/c++/12/ostream:166:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(long int) [with _CharT = char; _Traits = std::char_traits<char>; __ostream_type = std::basic_ostream<char>]’
  166 |       operator<<(long __n)
      |       ^~~~~~~~
/usr/include/c++/12/ostream:166:23: note:   no known conversion for argument 1 from ‘Cat’ to ‘long int’
  166 |       operator<<(long __n)

[...]

   30 |     std::cout << cat << std::endl;
      |                  ^~~
/usr/include/c++/12/ostream:667:5: note: candidate: ‘template<class _Traits> std::basic_ostream<wchar_t, _Traits>& std::operator<<(basic_ostream<wchar_t, _Traits>&, const char16_t*)’ (deleted)
  667 |     operator<<(basic_ostream<wchar_t, _Traits>&, const char16_t*) = delete;
      |     ^~~~~~~~
/usr/include/c++/12/ostream:667:5: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18: note:   mismatched types ‘wchar_t’ and ‘char’
   30 |     std::cout << cat << std::endl;
      |                  ^~~
/usr/include/c++/12/ostream:671:5: note: candidate: ‘template<class _Traits> std::basic_ostream<wchar_t, _Traits>& std::operator<<(basic_ostream<wchar_t, _Traits>&, const char32_t*)’ (deleted)
  671 |     operator<<(basic_ostream<wchar_t, _Traits>&, const char32_t*) = delete;
      |     ^~~~~~~~
/usr/include/c++/12/ostream:671:5: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18: note:   mismatched types ‘wchar_t’ and ‘char’
   30 |     std::cout << cat << std::endl;
      |                  ^~~
/usr/include/c++/12/ostream:754:5: note: candidate: ‘template<class _Ostream, class _Tp> _Ostream&& std::operator<<(_Ostream&&, const _Tp&)’
  754 |     operator<<(_Ostream&& __os, const _Tp& __x)
      |     ^~~~~~~~
/usr/include/c++/12/ostream:754:5: note:   template argument deduction/substitution failed:
/usr/include/c++/12/ostream: In substitution of ‘template<class _Ostream, class _Tp> _Ostream&& std::operator<<(_Ostream&&, const _Tp&) [with _Ostream = std::basic_ostream<char>&; _Tp = Cat]’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18:   required from here
/usr/include/c++/12/ostream:754:5: error: template constraint failure for ‘template<class _Os, class _Tp>  requires (__derived_from_ios_base<_Os>) && requires(_Os& __os, const _Tp& __t) {__os << __t;} using __rvalue_stream_insertion_t = _Os&&’
/usr/include/c++/12/ostream:754:5: note: constraints not satisfied
/usr/include/c++/12/ostream: In substitution of ‘template<class _Os, class _Tp>  requires (__derived_from_ios_base<_Os>) && requires(_Os& __os, const _Tp& __t) {__os << __t;} using __rvalue_stream_insertion_t = _Os&& [with _Os = std::basic_ostream<char>&; _Tp = Cat]’:
/usr/include/c++/12/ostream:754:5:   required by substitution of ‘template<class _Ostream, class _Tp> _Ostream&& std::operator<<(_Ostream&&, const _Tp&) [with _Ostream = std::basic_ostream<char>&; _Tp = Cat]’
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18:   required from here
/usr/include/c++/12/ostream:721:13:   required for the satisfaction of ‘__derived_from_ios_base<_Os>’ [with _Os = std::basic_ostream<char, std::char_traits<char> >&]
/usr/include/c++/12/ostream:721:39: note: the expression ‘is_class_v<_Tp> [with _Tp = std::basic_ostream<char, std::char_traits<char> >&]’ evaluated to ‘false’
  721 |     concept __derived_from_ios_base = is_class_v<_Tp>
      |                                       ^~~~~~~~~~~~~~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
