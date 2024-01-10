===================== Prompt =====================
This is my code:

File `/Users/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp`:
```
19     float const i = 3.14;
20     f(i);
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
/Users/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp:32:5: error: no matching function for call to 'f'
    f(i);
    ^
/Users/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp:26:6: note: candidate function not viable: no known conversion from 'const float' to 'char *' for 1st argument
void f(char*) {}

[...]

     ^
/Users/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-bind-const-ref-to-non-const-ref.cpp:28:6: note: candidate function not viable: 1st argument ('const float') would lose const qualifier
void f(float&) {}
     ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
