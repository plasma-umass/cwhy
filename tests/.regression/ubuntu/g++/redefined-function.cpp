===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/redefined-function.cpp`:
```
27             return x + y;
28         } else {
29             return x - y;
30         }
31     }
32     ```
33 */
34 int f(int x, int y) {
35     return x + y;
36 }
37 
38 int f(int x, int y) {
39     return x - y;
40 }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/redefined-function.cpp:38:5: error: redefinition of ‘int f(int, int)’
   38 | int f(int x, int y) {
      |     ^

[...]

/home/runner/work/cwhy/cwhy/tests/c++/redefined-function.cpp:34:5: note: ‘int f(int, int)’ previously defined here
   34 | int f(int x, int y) {
      |     ^
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
