/*
The problem is that you have defined the same function `int f(int x,
int y)` twice with different implementations. In C++, you cannot have
two functions with the same name and same parameter types in the same
scope. To fix this issue, you can either:

1. Provide different names for the functions if they serve different
purposes, e.g.,:

    ```cpp
    int add(int x, int y) {
        return x + y;
    }

    int subtract(int x, int y) {
        return x - y;
    }
    ```

2. If the functions have different behavior based on some condition,
you can use an additional parameter or a conditional statement within
a single function to choose between the two implementations:

   ```cpp
    int f(int x, int y, bool addition) {
        if (addition) {
            return x + y;
        } else {
            return x - y;
        }
    }
    ```
*/
int f(int x, int y) {
    return x + y;
}

int f(int x, int y) {
    return x - y;
}
