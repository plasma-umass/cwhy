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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/system_error`:
```
328   /** Write a std::error_code to an ostream.
329    *
330    * @relates error_code
331    * @since C++11
332    */
333   template<typename _CharT, typename _Traits>
334     basic_ostream<_CharT, _Traits>&
335     operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __e)
336     { return (__os << __e.category().name() << ':' << __e.value()); }
337 
338   /** Class error_condition
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:15: error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'Cat')
   30 |     std::cout << cat << std::endl;
      |     ~~~~~~~~~ ^  ~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/system_error:335:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const error_code' for 2nd argument
  335 |     operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __e)
      |     ^                                                ~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:517:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
  517 |     operator<<(basic_ostream<_CharT, _Traits>& __out, char __c)
      |     ^                                                 ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:523:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
  523 |     operator<<(basic_ostream<char, _Traits>& __out, char __c)
      |     ^                                               ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:534:5: note: candidate function template not viable: no known conversion from 'Cat' to 'signed char' for 2nd argument
  534 |     operator<<(basic_ostream<char, _Traits>& __out, signed char __c)
      |     ^                                               ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:539:5: note: candidate function template not viable: no known conversion from 'Cat' to 'unsigned char' for 2nd argument
  539 |     operator<<(basic_ostream<char, _Traits>& __out, unsigned char __c)
      |     ^                                               ~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:548:5: note: candidate function template not viable: no known conversion from 'Cat' to 'wchar_t' for 2nd argument
  548 |     operator<<(basic_ostream<char, _Traits>&, wchar_t) = delete;
      |     ^                                         ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:553:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char8_t' for 2nd argument
  553 |     operator<<(basic_ostream<char, _Traits>&, char8_t) = delete;
      |     ^                                         ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:558:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char16_t' for 2nd argument
  558 |     operator<<(basic_ostream<char, _Traits>&, char16_t) = delete;
      |     ^                                         ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:562:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char32_t' for 2nd argument
  562 |     operator<<(basic_ostream<char, _Traits>&, char32_t) = delete;
      |     ^                                         ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:615:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument
  615 |     operator<<(basic_ostream<char, _Traits>& __out, const char* __s)
      |     ^                                               ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:628:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const signed char *' for 2nd argument

[...]

  181 |       operator<<(unsigned short __n)
      |       ^          ~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:189:7: note: candidate function not viable: no known conversion from 'Cat' to 'int' for 1st argument
  189 |       operator<<(int __n);
      |       ^          ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:192:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned int' for 1st argument
  192 |       operator<<(unsigned int __n)
      |       ^          ~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:201:7: note: candidate function not viable: no known conversion from 'Cat' to 'long long' for 1st argument
  201 |       operator<<(long long __n)
      |       ^          ~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:205:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long long' for 1st argument
  205 |       operator<<(unsigned long long __n)
      |       ^          ~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:220:7: note: candidate function not viable: no known conversion from 'Cat' to 'double' for 1st argument
  220 |       operator<<(double __f)
      |       ^          ~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:224:7: note: candidate function not viable: no known conversion from 'Cat' to 'float' for 1st argument
  224 |       operator<<(float __f)
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:232:7: note: candidate function not viable: no known conversion from 'Cat' to 'long double' for 1st argument
  232 |       operator<<(long double __f)
      |       ^          ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:245:7: note: candidate function not viable: no known conversion from 'Cat' to 'const void *' for 1st argument; take the address of the argument with &
  245 |       operator<<(const void* __p)
      |       ^          ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:250:7: note: candidate function not viable: no known conversion from 'Cat' to 'nullptr_t' (aka 'std::nullptr_t') for 1st argument
  250 |       operator<<(nullptr_t)
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/ostream:283:7: note: candidate function not viable: no known conversion from 'Cat' to '__streambuf_type *' (aka 'basic_streambuf<char, std::char_traits<char>> *') for 1st argument
  283 |       operator<<(__streambuf_type* __sb);
      |       ^          ~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
