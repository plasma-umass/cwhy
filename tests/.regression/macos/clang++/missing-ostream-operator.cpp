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

File `/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/cstddef`:
```
118 template <class _Integer>
119 _LIBCPP_HIDE_FROM_ABI constexpr _EnableByteOverload<_Integer> &
120   operator<<=(byte& __lhs, _Integer __shift) noexcept
121   { return __lhs = __lhs << __shift; }
122 
123 template <class _Integer>
124 _LIBCPP_HIDE_FROM_ABI constexpr _EnableByteOverload<_Integer>
125   operator<< (byte  __lhs, _Integer __shift) noexcept
126   { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(__lhs) << __shift)); }
127 
128 template <class _Integer>
```


This is my error:
```
/Users/runner/work/cwhy/cwhy/tests/c++/missing-ostream-operator.cpp:30:15: error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'Cat')
    std::cout << cat << std::endl;
    ~~~~~~~~~ ^  ~~~
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/cstddef:125:3: note: candidate function template not viable: no known conversion from 'ostream' (aka 'basic_ostream<char>') to 'byte' for 1st argument
  operator<< (byte  __lhs, _Integer __shift) noexcept
  ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:800:1: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
operator<<(basic_ostream<_CharT, _Traits>& __os, char __cn)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:833:1: note: candidate function template not viable: no known conversion from 'Cat' to 'char' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, char __c)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:840:1: note: candidate function template not viable: no known conversion from 'Cat' to 'signed char' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, signed char __c)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:847:1: note: candidate function template not viable: no known conversion from 'Cat' to 'unsigned char' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, unsigned char __c)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:861:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument
operator<<(basic_ostream<_CharT, _Traits>& __os, const char* __strn)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:907:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const char *' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, const char* __str)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:914:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const signed char *' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, const signed char* __str)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:922:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const unsigned char *' for 2nd argument
operator<<(basic_ostream<char, _Traits>& __os, const unsigned char* __str)
^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:1107:1: note: candidate function template not viable: no known conversion from 'Cat' to 'const error_code' for 2nd argument
operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __ec)
^

[...]

    basic_ostream& operator<<(unsigned int __n);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:246:20: note: candidate function not viable: no known conversion from 'Cat' to 'long' for 1st argument
    basic_ostream& operator<<(long __n);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:247:20: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long' for 1st argument
    basic_ostream& operator<<(unsigned long __n);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:248:20: note: candidate function not viable: no known conversion from 'Cat' to 'long long' for 1st argument
    basic_ostream& operator<<(long long __n);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:249:20: note: candidate function not viable: no known conversion from 'Cat' to 'unsigned long long' for 1st argument
    basic_ostream& operator<<(unsigned long long __n);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:250:20: note: candidate function not viable: no known conversion from 'Cat' to 'float' for 1st argument
    basic_ostream& operator<<(float __f);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:251:20: note: candidate function not viable: no known conversion from 'Cat' to 'double' for 1st argument
    basic_ostream& operator<<(double __f);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:252:20: note: candidate function not viable: no known conversion from 'Cat' to 'long double' for 1st argument
    basic_ostream& operator<<(long double __f);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:253:20: note: candidate function not viable: no known conversion from 'Cat' to 'const void *' for 1st argument; take the address of the argument with &
    basic_ostream& operator<<(const void* __p);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:262:20: note: candidate function not viable: no known conversion from 'Cat' to 'basic_streambuf<char_type, traits_type> *' (aka 'basic_streambuf<char, std::char_traits<char>> *') for 1st argument
    basic_ostream& operator<<(basic_streambuf<char_type, traits_type>* __sb);
                   ^
/Applications/Xcode_15.4.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/ostream:268:20: note: candidate function not viable: no known conversion from 'Cat' to 'nullptr_t' (aka 'std::nullptr_t') for 1st argument
    basic_ostream& operator<<(nullptr_t)
                   ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
