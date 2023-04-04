# cwhy

by [Emery Berger](https://emeryberger.com), [Bryce Adelstein Lelbach](https://twitter.com/blelbach?lang=en), and [Nicolas van Kempen](https://nvankempen.com/)

("See why")

Explains and suggest fixes for C/C++/Rust compiler error messages.

## Installation

```
python3 -m pip install cwhy
```

## Usage

Just pipe your compiler's output to `cwhy`. `cwhy` will by default provide an explanation. If you'd like a suggested fix, add `--fix`.

e.g.,

```
% clang++ -g mycode.cpp |& cwhy         # explanation only
% clang++ -g mycode.cpp |& cwhy --fix   # to see a suggested fix
```

## Examples

### C++

<details>
<summary>
Expand to see the original (pretty obscure) error message:
</summary>

```
In file included from test/test.cpp:1:
In file included from /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/unordered_set:391:
In file included from /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/is_transparent.h:14:
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/type_traits:1838:38: error: implicit instantiation of undefined template 'std::hash<Q>'
    : public integral_constant<bool, __is_empty(_Tp)> {};
                                     ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__memory/compressed_pair.h:34:15: note: in instantiation of template class 'std::is_empty<std::hash<Q> >' requested here
              is_empty<_Tp>::value && !__libcpp_is_final<_Tp>::value>
              ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__memory/compressed_pair.h:110:35: note: in instantiation of default argument for '__compressed_pair_elem<std::hash<Q>, 1>' required here
                          private __compressed_pair_elem<_T2, 1> {
                                  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__hash_table:961:59: note: in instantiation of template class 'std::__compressed_pair<unsigned long, std::hash<Q> >' requested here
    __compressed_pair<size_type, hasher>                  __p2_;
                                                          ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/unordered_set:428:13: note: in instantiation of template class 'std::__hash_table<Q, std::hash<Q>, std::equal_to<Q>, std::allocator<Q> >' requested here
    __table __table_;
            ^
test/test.cpp:3:30: note: in instantiation of template class 'std::unordered_set<Q>' requested here
static std::unordered_set<Q> set;
                             ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__memory/shared_ptr.h:1710:50: note: template is declared here
template <class _Tp> struct _LIBCPP_TEMPLATE_VIS hash;
                                                 ^
1 error generated.
```
</details>

And here's the English-language explanation from <tt>cwhy</tt>:

```
% clang++ -g test/test.cpp |& cwhy
The error is caused by an implicit instantiation of an undefined
template 'std::hash<Q>'. It seems that the code is trying to use an
unordered_set with a custom type 'Q', but the standard library does
not know how to hash this type. The code needs to provide a hash
function for type 'Q' or specialize the 'std::hash' template for this
type.
```

And a proposed fix:

```
% clang++ test/test.cpp |& cwhy --fix
The problem is that `std::hash<Q>` is not defined, so it cannot be
implicitly instantiated. To fix the issue, define a hash function for
the `Q` struct. Here's an example:

    ```
    #include <unordered_set>

    struct Q {};

    namespace std {
        template<>
        struct hash<Q> {
            std::size_t operator()(const Q& q) const {
                return std::hash<const void*>{}(&q);
            }
        };
    }

    static std::unordered_set<Q> set;
    ```

This code explicitly specializes the `std::hash` template for type
`Q`, and provides a `std::size_t operator()` implementation for the
hash function. In this implementation, we use the `std::hash<const
void*>` hash function to compute the hash value of the address of the
`Q` object. This is a reasonable, albeit not very efficient, way to
compute a hash value for an object.
```



### Rust

```
% rustc test/testme.rs |& cwhy  
There are three issues:  1. There are two unused variables `x` in the
code.  2. The variable `x` is used after it has already been moved in
the call to `f(x)`, which takes ownership of `x`. 3. The function
`f(x)` takes ownership of `x`, which may not be necessary and could be
changed to borrow the value instead.
```
