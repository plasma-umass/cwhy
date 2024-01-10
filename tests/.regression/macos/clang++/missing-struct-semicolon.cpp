===================== Prompt =====================
This is my code:

File `/Users/runner/work/cwhy/cwhy/tests/c++/missing-struct-semicolon.cpp`:
```
 6 struct A {
 7 }; // Add semicolon here
 8 
 9 template <typename T>
10 ```
11 */
12 struct A {
13 }
14 
15 template <typename T>
16 void f() {
```


This is my error:
```
/Users/runner/work/cwhy/cwhy/tests/c++/missing-struct-semicolon.cpp:13:2: error: expected ';' after struct
}
 ^

[...]

 ;
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
