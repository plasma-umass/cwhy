===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
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
35     const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
36 }
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:21: error: expected ‘)’ before ‘<’ token
   11 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |           ~         ^
      |                     )
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:24:10: error: ‘span’ in namespace ‘std’ does not name a template type
   24 |     std::span<std::uint64_t> underlying;
      |          ^~~~
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp: In member function ‘uint64_t Matrix<M, N>::lowestRowSum() const’:
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:17:31: error: ‘underlying’ was not declared in this scope
   17 |             = std::accumulate(underlying.begin(), std::next(underlying.begin(), N), std::uint64_t(0));
      |                               ^~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp: In function ‘int main()’:

[...]

/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:35:45: error: no matching function for call to ‘Matrix<100, 10>::Matrix(std::vector<long unsigned int>&)’
   35 |     const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
      |                                             ^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:10:8: note: candidate: ‘constexpr Matrix<100, 10>::Matrix()’
   10 | struct Matrix {
      |        ^~~~~~
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:10:8: note:   candidate expects 0 arguments, 1 provided
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:10:8: note: candidate: ‘constexpr Matrix<100, 10>::Matrix(const Matrix<100, 10>&)’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:10:8: note:   no known conversion for argument 1 from ‘std::vector<long unsigned int>’ to ‘const Matrix<100, 10>&’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:10:8: note: candidate: ‘constexpr Matrix<100, 10>::Matrix(Matrix<100, 10>&&)’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:10:8: note:   no known conversion for argument 1 from ‘std::vector<long unsigned int>’ to ‘Matrix<100, 10>&&’
```


What's the problem?
==================================================
