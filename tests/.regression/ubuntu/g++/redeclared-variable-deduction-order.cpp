===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/redeclared-variable-deduction-order.cpp`:
```
 1 template <typename T>
 2 T f(T t) {
 3     return {};
 4 }
 5 
 6 extern decltype(f(0)) g;
 7 
 8 double f(int i) {
 9     return {};
10 }
11 
12 decltype(f(0)) g = {};
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/redeclared-variable-deduction-order.cpp:12:16: error: conflicting declaration ‘double g’
   12 | decltype(f(0)) g = {};
      |                ^

[...]

/home/runner/work/cwhy/cwhy/tests/c++/redeclared-variable-deduction-order.cpp:6:23: note: previous declaration as ‘int g’
    6 | extern decltype(f(0)) g;
      |                       ^
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
