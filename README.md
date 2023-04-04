# cwhy

("See why")

Explains and suggest fixes for C/C++/Rust compiler error messages.

## Examples

```
% clang++ -g test/test.cpp |& cwhy
The error is caused by an implicit instantiation of an undefined
template 'std::hash<Q>'. It seems that the code is trying to use an
unordered_set with a custom type 'Q', but the standard library does
not know how to hash this type. The code needs to provide a hash
function for type 'Q' or specialize the 'std::hash' template for this
type.
```

```
% rustc test/testme.rs |& cwhy  
There are three issues:  1. There are two unused variables `x` in the
code.  2. The variable `x` is used after it has already been moved in
the call to `f(x)`, which takes ownership of `x`. 3. The function
`f(x)` takes ownership of `x`, which may not be necessary and could be
changed to borrow the value instead.
```
