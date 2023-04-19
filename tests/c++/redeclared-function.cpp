/*
The problem is that you have declared the function `f()` with an `int`
return type, but then you have defined it with a `void` return type.
The return types should be the same for both declaration and
definition.

To fix this issue, change the declaration and definition to have the
same return type. For example, if you want the function to return an
`int`, modify the code as follows:

```
int f();

int f() {
  // Add your function implementation here
}
```

Or, if you want the function to return `void`, modify the code like
this:

```
void f();

void f() {
  // Add your function implementation here
}
```
*/
int f();

void f() {}
