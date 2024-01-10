===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp`:
```
21 }
22 ```
23 
24 With this change, your code should compile without any issues.
25 */
26 void f(char*) {}
27 
28 void f(float&) {}
29 
30 int main() {
31     float const i = 3.14;
32     f(i);
33 }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp:32:7: error: binding reference of type ‘float&’ to ‘const float’ discards qualifiers
   32 |     f(i);
      |       ^

[...]

/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp:28:8: note:   initializing argument 1 of ‘void f(float&)’
   28 | void f(float&) {}
      |        ^~~~~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
