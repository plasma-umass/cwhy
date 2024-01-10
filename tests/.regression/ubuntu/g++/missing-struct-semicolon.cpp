===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/missing-struct-semicolon.cpp`:
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
/home/runner/work/cwhy/cwhy/tests/c++/missing-struct-semicolon.cpp:13:2: error: expected ‘;’ after struct definition
   13 | }

[...]

      |  ^
      |  ;
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
