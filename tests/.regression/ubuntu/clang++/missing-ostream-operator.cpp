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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/cstddef`:
```
119   template<typename _IntegerType>
120     using __byte_op_t = typename __byte_operand<_IntegerType>::__type;
121 
122   template<typename _IntegerType>
123     [[__gnu__::__always_inline__]]
124     constexpr __byte_op_t<_IntegerType>
125     operator<<(byte __b, _IntegerType __shift) noexcept
126     { return (byte)(unsigned char)((unsigned)__b << __shift); }
127 
128   template<typename _IntegerType>
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/system_error`:
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
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/cstddef:125:5: note: candidate function template not viable: no known conversion from 'ostream' (aka 'basic_ostream<char>') to 'byte' for 1st argument
  125 |     operator<<(byte __b, _IntegerType __shift) noexcept
      |     ^          ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/system_error:339:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const error_code' for 2nd argument
  339 |     operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __e)
      |     ^                                                ~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:570:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
  570 |     operator<<(basic_ostream<_CharT, _Traits>& __out, char __c)
      |     ^                                                 ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:576:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
  576 |     operator<<(basic_ostream<char, _Traits>& __out, char __c)
      |     ^                                               ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:587:5: note: candidate function template not viable: no known conversion from 'Cat' to 'signed char' for 2nd argument
  587 |     operator<<(basic_ostream<char, _Traits>& __out, signed char __c)
      |     ^                                               ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:592:5: note: candidate function template not viable: no known conversion from 'Cat' to 'unsigned char' for 2nd argument
  592 |     operator<<(basic_ostream<char, _Traits>& __out, unsigned char __c)
      |     ^                                               ~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:601:5: note: candidate function template not viable: no known conversion from 'Cat' to 'wchar_t' for 2nd argument
  601 |     operator<<(basic_ostream<char, _Traits>&, wchar_t) = delete;
      |     ^                                         ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:606:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char8_t' for 2nd argument
  606 |     operator<<(basic_ostream<char, _Traits>&, char8_t) = delete;
      |     ^                                         ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:611:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char16_t' for 2nd argument
  611 |     operator<<(basic_ostream<char, _Traits>&, char16_t) = delete;
      |     ^                                         ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:615:5: note: candidate function template not viable: no known conversion from 'Cat' to 'char32_t' for 2nd argument
  615 |     operator<<(basic_ostream<char, _Traits>&, char32_t) = delete;
      |     ^                                         ~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:668:5: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument

[...]

  189 |       operator<<(unsigned short __n)
      |       ^          ~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:197:7: note: candidate function not viable: no known conversion from 'Cat' to 'int' for 1st argument
  197 |       operator<<(int __n);
      |       ^          ~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:200:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned int' for 1st argument
  200 |       operator<<(unsigned int __n)
      |       ^          ~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:209:7: note: candidate function not viable: no known conversion from 'Cat' to 'long long' for 1st argument
  209 |       operator<<(long long __n)
      |       ^          ~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:213:7: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long long' for 1st argument
  213 |       operator<<(unsigned long long __n)
      |       ^          ~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:228:7: note: candidate function not viable: no known conversion from 'Cat' to 'double' for 1st argument
  228 |       operator<<(double __f)
      |       ^          ~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:232:7: note: candidate function not viable: no known conversion from 'Cat' to 'float' for 1st argument
  232 |       operator<<(float __f)
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:240:7: note: candidate function not viable: no known conversion from 'Cat' to 'long double' for 1st argument
  240 |       operator<<(long double __f)
      |       ^          ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:298:7: note: candidate function not viable: no known conversion from 'Cat' to 'const void *' for 1st argument; take the address of the argument with &
  298 |       operator<<(const void* __p)
      |       ^          ~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:303:7: note: candidate function not viable: no known conversion from 'Cat' to 'nullptr_t' (aka 'std::nullptr_t') for 1st argument
  303 |       operator<<(nullptr_t)
      |       ^          ~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/ostream:336:7: note: candidate function not viable: no known conversion from 'Cat' to '__streambuf_type *' (aka 'basic_streambuf<char, std::char_traits<char>> *') for 1st argument
  336 |       operator<<(__streambuf_type* __sb);
      |       ^          ~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
