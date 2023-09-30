===================== Prompt =====================
This is my code:

File `/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/span`:
```
418               enable_if_t<is_convertible_v<const _OtherElementType(*)[], element_type (*)[]>, nullptr_t> = nullptr>
419     _LIBCPP_INLINE_VISIBILITY
420     constexpr span(const array<_OtherElementType, _Sz>& __arr) noexcept : __data{__arr.data()}, __size{_Sz} {}
421 
422     template <class _Container>
423     _LIBCPP_INLINE_VISIBILITY
424         constexpr span(      _Container& __c,
425             enable_if_t<__is_span_compatible_container<_Container, _Tp>::value, nullptr_t> = nullptr)
426         : __data{_VSTD::data(__c)}, __size{(size_type) _VSTD::size(__c)} {}
427 
428     template <class _Container>
```

File `/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
 4 #include <iterator>
 5 #include <numeric>
 6 #include <span>
 7 #include <vector>
 8 
 9 template <std::uint32_t M, std::uint32_t N>
10 struct Matrix {
11     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
12         assert(underlying.size() == M * N);
13     }
14 
15     std::uint64_t lowestRowSum() const {
16         const auto firstRowSum
17             = std::accumulate(underlying.begin(), std::next(underlying.begin(), N), std::uint64_t(0));
18         const auto subMatrix = Matrix<M - 1, N>(underlying.subspan(N));
19         return std::min(firstRowSum, subMatrix.lowestRowSum());
20     }
21 
22   private:
```

File `/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
30 int main() {
31     std::vector<std::uint64_t> v(M * N);
32     for (int i = 0; i < M * N; ++i) {
33         v[i] = i;
34     }
35     const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
36 }
```


This is my error:
```
In file included from /Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:6:
/Applications/Xcode_14.2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/span:425:71: fatal error: recursive template instantiation exceeded maximum depth of 1024
            enable_if_t<__is_span_compatible_container<_Container, _Tp>::value, nullptr_t> = nullptr)
                        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:51: note: while substituting deduced template arguments into function template 'span' [with _Container = std::span<unsigned long long, 18446744073709551615>]
    Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
                                                  ^
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:18:32: note: in instantiation of member function 'Matrix<4294966373, 10>::Matrix' requested here
        const auto subMatrix = Matrix<M - 1, N>(underlying.subspan(N));
                               ^
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<4294966374, 10>::lowestRowSum' requested here
        return std::min(firstRowSum, subMatrix.lowestRowSum());

[...]

                                               ^
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<4294966375, 10>::lowestRowSum' requested here
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<4294966376, 10>::lowestRowSum' requested here
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: (skipping 1015 contexts in backtrace; use -ftemplate-backtrace-limit=0 to see all)
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<96, 10>::lowestRowSum' requested here
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<97, 10>::lowestRowSum' requested here
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<98, 10>::lowestRowSum' requested here
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:48: note: in instantiation of member function 'Matrix<99, 10>::lowestRowSum' requested here
/Users/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:35:47: note: in instantiation of member function 'Matrix<100, 10>::lowestRowSum' requested here
    const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
                                              ^
1 error generated.
```


What's the problem?
==================================================
