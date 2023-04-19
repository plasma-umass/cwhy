/*
The problem is that you are missing a semicolon after the closing
brace of the struct definition. The correct code should be:

```cpp
struct A {
}; // Add semicolon here

template <typename T>
```
*/
struct A {
}

template <typename T>
void f() {
}
