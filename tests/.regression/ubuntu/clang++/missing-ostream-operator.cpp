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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cstddef`:
```
118   template<typename _IntegerType>
119     using __byte_op_t = typename __byte_operand<_IntegerType>::__type;
120 
121   template<typename _IntegerType>
122     [[__gnu__::__always_inline__]]
123     constexpr __byte_op_t<_IntegerType>
124     operator<<(byte __b, _IntegerType __shift) noexcept
125     { return (byte)(unsigned char)((unsigned)__b << __shift); }
126 
127   template<typename _IntegerType>
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/system_error`:
```
332   /** Write a std::error_code to an ostream.
333    *
334    * @relates error_code
335    * @since C++11
336    */
337   template<typename _CharT, typename _Traits>
338     basic_ostream<_CharT, _Traits>&
339     operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __e)
340     { return (__os << __e.category().name() << ':' << __e.value()); }
341 
342   /** Class error_condition
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:15: error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'Cat')
   30 |     std::cout << cat << std::endl;
      |     ~~~~~~~~~ ^  ~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cstddef:124:5: note: candidate function template not viable: no known conversion from 'ostream' (aka 'basic_ostream<char>') to 'byte' for 1st argument
  124 |     operator<<(byte __b, _IntegerType __shift) noexcept
      |     ^          ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/system_error:339:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const error_code' for 2nd argument
  339 |     operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __e)
      |     ^                                                ~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:564:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
  564 |     operator<<(basic_ostream<_CharT, _Traits>& __out, char __c)
      |     ^                                                 ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:570:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
  570 |     operator<<(basic_ostream<char, _Traits>& __out, char __c)
      |     ^                                               ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:581:5: note: candidate function template not viable: no known conversion from 'Cat' to 'signed char' for 2nd argument
  581 |     operator<<(basic_ostream<char, _Traits>& __out, signed char __c)
      |     ^                                               ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:586:5: note: candidate function template not viable: no known conversion from 'Cat' to 'unsigned char' for 2nd argument
  586 |     operator<<(basic_ostream<char, _Traits>& __out, unsigned char __c)
      |     ^                                               ~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:595:5: note: candidate function template not viable: no known conversion from 'Cat' to 'wchar_t' for 2nd argument
  595 |     operator<<(basic_ostream<char, _Traits>&, wchar_t) = delete;
      |     ^                                         ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:600:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char8_t' for 2nd argument
  600 |     operator<<(basic_ostream<char, _Traits>&, char8_t) = delete;
      |     ^                                         ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:605:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char16_t' for 2nd argument
  605 |     operator<<(basic_ostream<char, _Traits>&, char16_t) = delete;
      |     ^                                         ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:609:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char32_t' for 2nd argument
  609 |     operator<<(basic_ostream<char, _Traits>&, char32_t) = delete;
      |     ^                                         ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:662:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument
  662 |     operator<<(basic_ostream<char, _Traits>& __out, const char* __s)
      |     ^                                               ~~~~~~~~~~~~~~~

[...]

  180 |       operator<<(short __n);
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:183:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned short' for 1st argument
  183 |       operator<<(unsigned short __n)
      |       ^          ~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:191:7: note: candidate function not viable: no known conversion from 'Cat' to 'int' for 1st argument
  191 |       operator<<(int __n);
      |       ^          ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:194:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned int' for 1st argument
  194 |       operator<<(unsigned int __n)
      |       ^          ~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:203:7: note: candidate function not viable: no known conversion from 'Cat' to 'long long' for 1st argument
  203 |       operator<<(long long __n)
      |       ^          ~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:207:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long long' for 1st argument
  207 |       operator<<(unsigned long long __n)
      |       ^          ~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:222:7: note: candidate function not viable: no known conversion from 'Cat' to 'double' for 1st argument
  222 |       operator<<(double __f)
      |       ^          ~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:226:7: note: candidate function not viable: no known conversion from 'Cat' to 'float' for 1st argument
  226 |       operator<<(float __f)
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:234:7: note: candidate function not viable: no known conversion from 'Cat' to 'long double' for 1st argument
  234 |       operator<<(long double __f)
      |       ^          ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:292:7: note: candidate function not viable: no known conversion from 'Cat' to 'const void *' for 1st argument; take the address of the argument with &
  292 |       operator<<(const void* __p)
      |       ^          ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:297:7: note: candidate function not viable: no known conversion from 'Cat' to 'nullptr_t' (aka 'std::nullptr_t') for 1st argument
  297 |       operator<<(nullptr_t)
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/ostream:330:7: note: candidate function not viable: no known conversion from 'Cat' to '__streambuf_type *' (aka 'basic_streambuf<char, std::char_traits<char>> *') for 1st argument
  330 |       operator<<(__streambuf_type* __sb);
      |       ^          ~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
