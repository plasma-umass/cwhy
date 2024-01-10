===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp`:
```
 1 void f(void*) {}
 2 void f(char*) {}
 3 
 4 template <typename T>
 5 auto g(T t) -> decltype(f(t)) {
 6     return f(t);
 7 }
 8 
 9 int main() {
10     g(nullptr);
11 }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:10:6: error: no matching function for call to ‘g(std::nullptr_t)’
   10 |     g(nullptr);
      |     ~^~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:5:6: note: candidate: ‘template<class T> decltype (f(t)) g(T)’
    5 | auto g(T t) -> decltype(f(t)) {
      |      ^
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:5:6: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp: In substitution of ‘template<class T> decltype (f(t)) g(T) [with T = std::nullptr_t]’:
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:10:6:   required from here

[...]

/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:5:26: error: call of overloaded ‘f(std::nullptr_t&)’ is ambiguous
    5 | auto g(T t) -> decltype(f(t)) {
      |                         ~^~~
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:1:6: note: candidate: ‘void f(void*)’
    1 | void f(void*) {}
      |      ^
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-ambiguous.cpp:2:6: note: candidate: ‘void f(char*)’
    2 | void f(char*) {}
      |      ^
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
