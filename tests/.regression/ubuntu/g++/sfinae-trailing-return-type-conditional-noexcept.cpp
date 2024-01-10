===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp`:
```
 1 template <typename T>
 2 T f(T t0, T t1) {
 3     return {};
 4 }
 5 
 6 template <typename T>
 7 auto g(T t) noexcept(noexcept(f(t))) -> decltype(f(t)) {
 8     return f(t);
 9 }
10 
11 int main() {
12     g(17);
13 }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:12:6: error: no matching function for call to ‘g(int)’
   12 |     g(17);
      |     ~^~~~
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:7:6: note: candidate: ‘template<class T> decltype (f(t)) g(T)’
    7 | auto g(T t) noexcept(noexcept(f(t))) -> decltype(f(t)) {
      |      ^
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:7:6: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp: In substitution of ‘template<class T> decltype (f(t)) g(T) [with T = int]’:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:12:6:   required from here

[...]

/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:7:51: error: no matching function for call to ‘f(int&)’
    7 | auto g(T t) noexcept(noexcept(f(t))) -> decltype(f(t)) {
      |                                                  ~^~~
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:2:3: note: candidate: ‘template<class T> T f(T, T)’
    2 | T f(T t0, T t1) {
      |   ^
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:2:3: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:7:51: note:   candidate expects 2 arguments, 1 provided
    7 | auto g(T t) noexcept(noexcept(f(t))) -> decltype(f(t)) {
      |                                                  ~^~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
