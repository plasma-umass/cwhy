===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/redeclared-function.cpp`:
```
23 void f();
24 
25 void f() {
26   // Add your function implementation here
27 }
28 ```
29 */
30 int f();
31 
32 void f() {}
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/redeclared-function.cpp:32:6: error: ambiguating new declaration of ‘void f()’
   32 | void f() {}
      |      ^

[...]

/home/runner/work/cwhy/cwhy/tests/c++/redeclared-function.cpp:30:5: note: old declaration ‘int f()’
   30 | int f();
      |     ^
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
