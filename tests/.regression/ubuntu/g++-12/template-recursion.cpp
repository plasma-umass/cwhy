===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
 4 #include <cstdint>
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
24     // The Matrix object does not own the underlying memory, and is just used to perform operations.
25     std::span<std::uint64_t> underlying;
26 };
27 
28 #define M 100
29 #define N 10
30 
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
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:12:21: error: expected ‘)’ before ‘<’ token
   12 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |           ~         ^
      |                     )
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:25:10: error: ‘span’ in namespace ‘std’ does not name a template type
   25 |     std::span<std::uint64_t> underlying;
      |          ^~~~
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp: In member function ‘uint64_t Matrix<M, N>::lowestRowSum() const’:
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:18:31: error: ‘underlying’ was not declared in this scope
   18 |             = std::accumulate(underlying.begin(), std::next(underlying.begin(), N), std::uint64_t(0));
      |                               ^~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp: In function ‘int main()’:

[...]

/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:36:45: error: no matching function for call to ‘Matrix<100, 10>::Matrix(std::vector<long unsigned int>&)’
   36 |     const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
      |                                             ^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:8: note: candidate: ‘constexpr Matrix<100, 10>::Matrix()’
   11 | struct Matrix {
      |        ^~~~~~
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:8: note:   candidate expects 0 arguments, 1 provided
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:8: note: candidate: ‘constexpr Matrix<100, 10>::Matrix(const Matrix<100, 10>&)’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:8: note:   no known conversion for argument 1 from ‘std::vector<long unsigned int>’ to ‘const Matrix<100, 10>&’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:8: note: candidate: ‘constexpr Matrix<100, 10>::Matrix(Matrix<100, 10>&&)’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:8: note:   no known conversion for argument 1 from ‘std::vector<long unsigned int>’ to ‘Matrix<100, 10>&&’
```


What's the problem?
==================================================
