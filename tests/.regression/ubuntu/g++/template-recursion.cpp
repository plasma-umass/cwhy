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
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp: In instantiation of ‘Matrix<M, N>::Matrix(std::span<long unsigned int>) [with unsigned int M = 4294966497; unsigned int N = 10]’:
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:60:   recursively required from ‘uint64_t Matrix<M, N>::lowestRowSum() const [with unsigned int M = 99; unsigned int N = 10]’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:20:60:   required from ‘uint64_t Matrix<M, N>::lowestRowSum() const [with unsigned int M = 100; unsigned int N = 10]’
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:36:59:   required from here

[...]

/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:12:51: fatal error: template instantiation depth exceeds maximum of 900 (use ‘-ftemplate-depth=’ to increase the maximum)
   12 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |                                                   ^~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
