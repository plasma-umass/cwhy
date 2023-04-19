/*
The problem is that you are trying to pass a constant float `i` as an
argument to the function `f()`, but there is no suitable overload for
the function `f()` that can accept a constant float as its parameter.

The two overloads you have for function `f()` are:

1. `void f(char*)` 2. `void f(float&)`

None of them matches the argument type `const float`.

To fix this, you can add another overload of the `f()` function that
takes a `const float&` as its argument:

```cpp
void f(const float&) {}

int main() {
    float const i = 3.14;
    f(i);
}
```

With this change, your code should compile without any issues.
*/
void f(char*) {}

void f(float&) {}

int main() {
    float const i = 3.14;
    f(i);
}
