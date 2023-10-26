===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
 2 #include <cassert>
 3 #include <cstddef>
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
23     // The Matrix object does not own the underlying memory, and is just used to perform operations.
24     std::span<std::uint64_t> underlying;
25 };
26 
27 #define M 100
28 #define N 10
29 
30 int main() {
31     std::vector<std::uint64_t> v(M * N);
32     for (int i = 0; i < M * N; ++i) {
33         v[i] = i;
34     }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:9:16: error: no type named 'uint32_t' in namespace 'std'
    9 | template <std::uint32_t M, std::uint32_t N>
      |           ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:9:33: error: no type named 'uint32_t' in namespace 'std'
    9 | template <std::uint32_t M, std::uint32_t N>
      |                            ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:17: error: no template named 'span' in namespace 'std'
   11 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |            ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:27: error: no member named 'uint64_t' in namespace 'std'
   11 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |                      ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:15:10: error: no type named 'uint64_t' in namespace 'std'
   15 |     std::uint64_t lowestRowSum() const {

[...]

      |     ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:24:10: error: no template named 'span' in namespace 'std'
   24 |     std::span<std::uint64_t> underlying;
      |     ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:24:20: error: no member named 'uint64_t' in namespace 'std'
   24 |     std::span<std::uint64_t> underlying;
      |               ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:17:90: error: no member named 'uint64_t' in namespace 'std'
   17 |             = std::accumulate(underlying.begin(), std::next(underlying.begin(), N), std::uint64_t(0));
      |                                                                                     ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:31:22: error: no member named 'uint64_t' in namespace 'std'
   31 |     std::vector<std::uint64_t> v(M * N);
      |                 ~~~~~^
9 errors generated.
```


What's the problem?
==================================================
