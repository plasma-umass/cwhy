===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
 5 #include <iterator>
 6 #include <numeric>
 7 #include <span>
 8 #include <vector>
 9 
10 template <std::uint32_t M, std::uint32_t N>
11 struct Matrix {
12     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
13         assert(underlying.size() == M * N);
14     }
15 
16     std::uint64_t lowestRowSum() const {
17         const auto firstRowSum
18             = std::accumulate(underlying.begin(), std::next(underlying.begin(), N), std::uint64_t(0));
19         const auto subMatrix = Matrix<M - 1, N>(underlying.subspan(N));
20         return std::min(firstRowSum, subMatrix.lowestRowSum());
21     }
22 
23   private:
```

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
31 int main() {
32     std::vector<std::uint64_t> v(M * N);
33     for (int i = 0; i < M * N; ++i) {
34         v[i] = i;
35     }
36     const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
37 }
```


This is my error:
```
fatal error: recursive template instantiation exceeded maximum depth of 1024
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:12:51: note: while substituting deduced template arguments into function template 'span' [with _Range = std::span<std::uint64_t> &]
   12 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |                                                   ^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:19:32: note: in instantiation of member function 'Matrix<4294966373, 10>::Matrix' requested here
   19 |         const auto subMatrix = Matrix<M - 1, N>(underlying.subspan(N));
      |                                ^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<4294966374, 10>::lowestRowSum' requested here
   20 |         return std::min(firstRowSum, subMatrix.lowestRowSum());
      |                                                ^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<4294966375, 10>::lowestRowSum' requested here

[...]

/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<4294966376, 10>::lowestRowSum' requested here
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: (skipping 1015 contexts in backtrace; use -ftemplate-backtrace-limit=0 to see all)
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<96, 10>::lowestRowSum' requested here
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<97, 10>::lowestRowSum' requested here
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<98, 10>::lowestRowSum' requested here
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:48: note: in instantiation of member function 'Matrix<99, 10>::lowestRowSum' requested here
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:36:47: note: in instantiation of member function 'Matrix<100, 10>::lowestRowSum' requested here
   36 |     const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
      |                                               ^
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
