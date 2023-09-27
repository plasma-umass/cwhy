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
In file included from /usr/include/c++/9/iostream:39,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:18:
/usr/include/c++/9/ostream:108:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(std::basic_ostream<_CharT, _Traits>::__ostream_type& (*)(std::basic_ostream<_CharT, _Traits>::__ostream_type&)) [with _CharT = char; _Traits = std::char_traits<char>; std::basic_ostream<_CharT, _Traits>::__ostream_type = std::basic_ostream<char>]’
  108 |       operator<<(__ostream_type& (*__pf)(__ostream_type&))
      |       ^~~~~~~~
/usr/include/c++/9/ostream:108:36: note:   no known conversion for argument 1 from ‘Cat’ to ‘std::basic_ostream<char>::__ostream_type& (*)(std::basic_ostream<char>::__ostream_type&)’ {aka ‘std::basic_ostream<char>& (*)(std::basic_ostream<char>&)’}
  108 |       operator<<(__ostream_type& (*__pf)(__ostream_type&))
      |                  ~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/9/ostream:117:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(std::basic_ostream<_CharT, _Traits>::__ios_type& (*)(std::basic_ostream<_CharT, _Traits>::__ios_type&)) [with _CharT = char; _Traits = std::char_traits<char>; std::basic_ostream<_CharT, _Traits>::__ostream_type = std::basic_ostream<char>; std::basic_ostream<_CharT, _Traits>::__ios_type = std::basic_ios<char>]’
  117 |       operator<<(__ios_type& (*__pf)(__ios_type&))
      |       ^~~~~~~~
/usr/include/c++/9/ostream:117:32: note:   no known conversion for argument 1 from ‘Cat’ to ‘std::basic_ostream<char>::__ios_type& (*)(std::basic_ostream<char>::__ios_type&)’ {aka ‘std::basic_ios<char>& (*)(std::basic_ios<char>&)’}
  117 |       operator<<(__ios_type& (*__pf)(__ios_type&))
      |                  ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~
/usr/include/c++/9/ostream:127:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(std::ios_base& (*)(std::ios_base&)) [with _CharT = char; _Traits = std::char_traits<char>; std::basic_ostream<_CharT, _Traits>::__ostream_type = std::basic_ostream<char>]’
  127 |       operator<<(ios_base& (*__pf) (ios_base&))
      |       ^~~~~~~~
/usr/include/c++/9/ostream:127:30: note:   no known conversion for argument 1 from ‘Cat’ to ‘std::ios_base& (*)(std::ios_base&)’
  127 |       operator<<(ios_base& (*__pf) (ios_base&))
      |                  ~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/c++/9/ostream:166:7: note: candidate: ‘std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(long int) [with _CharT = char; _Traits = std::char_traits<char>; std::basic_ostream<_CharT, _Traits>::__ostream_type = std::basic_ostream<char>]’
  166 |       operator<<(long __n)
      |       ^~~~~~~~

[...]

   30 |     std::cout << cat << std::endl;
      |                  ^~~
In file included from /usr/include/c++/9/iostream:39,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:18:
/usr/include/c++/9/ostream:578:5: note: candidate: ‘template<class _Traits> std::basic_ostream<char, _Traits>& std::operator<<(std::basic_ostream<char, _Traits>&, const signed char*)’
  578 |     operator<<(basic_ostream<char, _Traits>& __out, const signed char* __s)
      |     ^~~~~~~~
/usr/include/c++/9/ostream:578:5: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18: note:   cannot convert ‘cat’ (type ‘Cat’) to type ‘const signed char*’
   30 |     std::cout << cat << std::endl;
      |                  ^~~
In file included from /usr/include/c++/9/iostream:39,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:18:
/usr/include/c++/9/ostream:583:5: note: candidate: ‘template<class _Traits> std::basic_ostream<char, _Traits>& std::operator<<(std::basic_ostream<char, _Traits>&, const unsigned char*)’
  583 |     operator<<(basic_ostream<char, _Traits>& __out, const unsigned char* __s)
      |     ^~~~~~~~
/usr/include/c++/9/ostream:583:5: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18: note:   cannot convert ‘cat’ (type ‘Cat’) to type ‘const unsigned char*’
   30 |     std::cout << cat << std::endl;
      |                  ^~~
In file included from /usr/include/c++/9/iostream:39,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:18:
/usr/include/c++/9/ostream:691:5: note: candidate: ‘template<class _Ostream, class _Tp> typename std::enable_if<std::__and_<std::__not_<std::is_lvalue_reference<_Tp> >, std::__is_convertible_to_basic_ostream<_Ostream>, std::__is_insertable<typename std::__is_convertible_to_basic_ostream<_Tp>::__ostream_type, const _Tp&, void> >::value, typename std::__is_convertible_to_basic_ostream<_Tp>::__ostream_type>::type std::operator<<(_Ostream&&, const _Tp&)’
  691 |     operator<<(_Ostream&& __os, const _Tp& __x)
      |     ^~~~~~~~
/usr/include/c++/9/ostream:691:5: note:   template argument deduction/substitution failed:
/usr/include/c++/9/ostream: In substitution of ‘template<class _Ostream, class _Tp> typename std::enable_if<std::__and_<std::__not_<std::is_lvalue_reference<_Tp> >, std::__is_convertible_to_basic_ostream<_Ostream>, std::__is_insertable<typename std::__is_convertible_to_basic_ostream<_Tp>::__ostream_type, const _Tp&, void> >::value, typename std::__is_convertible_to_basic_ostream<_Tp>::__ostream_type>::type std::operator<<(_Ostream&&, const _Tp&) [with _Ostream = std::basic_ostream<char>&; _Tp = Cat]’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:18:   required from here
/usr/include/c++/9/ostream:691:5: error: no type named ‘type’ in ‘struct std::enable_if<false, std::basic_ostream<char>&>’
```


What's the problem?
==================================================