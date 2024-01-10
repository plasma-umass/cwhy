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
/home/runner/work/cwhy/cwhy/tests/c++/redeclared-variable-deduction-order.cpp:12:16: error: redefinition of 'g' with a different type: 'decltype(f(0))' (aka 'double') vs 'decltype(f(0))' (aka 'int')
   12 | decltype(f(0)) g = {};
      |                ^
/home/runner/work/cwhy/cwhy/tests/c++/redeclared-variable-deduction-order.cpp:6:23: note: previous declaration is here

[...]

    6 | extern decltype(f(0)) g;
      |                       ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
