===================== Prompt =====================
This is my code:

File `/Users/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp`:
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

File `/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/cstddef`:
```
136 template <class _Integer>
137   constexpr _EnableByteOverload<_Integer> &
138   operator<<=(byte& __lhs, _Integer __shift) noexcept
139   { return __lhs = __lhs << __shift; }
140 
141 template <class _Integer>
142   constexpr _EnableByteOverload<_Integer>
143   operator<< (byte  __lhs, _Integer __shift) noexcept
144   { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(__lhs) << __shift)); }
145 
146 template <class _Integer>
```


This is my error:
```
/Users/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:15: error: invalid operands to binary expression ('std::ostream' (aka 'basic_ostream<char>') and 'Cat')
    std::cout << cat << std::endl;
    ~~~~~~~~~ ^  ~~~
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/cstddef:143:3: note: candidate function template not viable: no known conversion from 'std::ostream' (aka 'basic_ostream<char>') to 'std::byte' for 1st argument
  operator<< (byte  __lhs, _Integer __shift) noexcept
  ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:748:1: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
operator<<(basic_ostream<_CharT, _Traits>& __os, char __cn)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:781:1: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, char __c)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:788:1: note: candidate function template not viable: no known conversion from 'Cat' to 'signed char' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, signed char __c)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:795:1: note: candidate function template not viable: no known conversion from 'Cat' to 'unsigned char' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, unsigned char __c)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:809:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument
operator<<(basic_ostream<_CharT, _Traits>& __os, const char* __strn)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:855:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, const char* __str)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:862:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const signed char *' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, const signed char* __str)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:870:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const unsigned char *' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, const unsigned char* __str)
^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:1055:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const std::error_code' for 2nd argument
operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __ec)
^

[...]

    basic_ostream& operator<<(unsigned int __n);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:205:20: note: candidate function not viable: no known conversion from 'Cat' to 'long' for 1st argument
    basic_ostream& operator<<(long __n);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:206:20: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long' for 1st argument
    basic_ostream& operator<<(unsigned long __n);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:207:20: note: candidate function not viable: no known conversion from 'Cat' to 'long long' for 1st argument
    basic_ostream& operator<<(long long __n);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:208:20: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long long' for 1st argument
    basic_ostream& operator<<(unsigned long long __n);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:209:20: note: candidate function not viable: no known conversion from 'Cat' to 'float' for 1st argument
    basic_ostream& operator<<(float __f);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:210:20: note: candidate function not viable: no known conversion from 'Cat' to 'double' for 1st argument
    basic_ostream& operator<<(double __f);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:211:20: note: candidate function not viable: no known conversion from 'Cat' to 'long double' for 1st argument
    basic_ostream& operator<<(long double __f);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:212:20: note: candidate function not viable: no known conversion from 'Cat' to 'const void *' for 1st argument; take the address of the argument with &
    basic_ostream& operator<<(const void* __p);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:213:20: note: candidate function not viable: no known conversion from 'Cat' to 'basic_streambuf<std::basic_ostream<char>::char_type, std::basic_ostream<char>::traits_type> *' (aka 'basic_streambuf<char, std::char_traits<char>> *') for 1st argument
    basic_ostream& operator<<(basic_streambuf<char_type, traits_type>* __sb);
                   ^
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:216:20: note: candidate function not viable: no known conversion from 'Cat' to 'std::nullptr_t' for 1st argument
    basic_ostream& operator<<(nullptr_t)
                   ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
