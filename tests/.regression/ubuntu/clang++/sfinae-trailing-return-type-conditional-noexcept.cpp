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
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:12:5: error: no matching function for call to 'g'
   12 |     g(17);
      |     ^
/home/runner/work/cwhy/cwhy/tests/c++/sfinae-trailing-return-type-conditional-noexcept.cpp:7:6: note: candidate template ignored: substitution failure [with T = int]: no matching function for call to 'f'

[...]

    7 | auto g(T t) noexcept(noexcept(f(t))) -> decltype(f(t)) {
      |      ^                                           ~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
