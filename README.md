# c-why

Explains and suggest fixes for C/C++ error messages

## Example

```
% clang++ -g test/test.cpp |& cwhy
The error is caused by an implicit instantiation of an undefined
template 'std::hash<Q>'. It seems that the code is trying to use an
unordered_set with a custom type 'Q', but the standard library does
not know how to hash this type. The code needs to provide a hash
function for type 'Q' or specialize the 'std::hash' template for this
type.
```