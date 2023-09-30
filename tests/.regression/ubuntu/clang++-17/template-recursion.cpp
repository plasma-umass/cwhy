===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
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
```

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
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
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:11:17: error: no template named 'span' in namespace 'std'
   11 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |            ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:24:10: error: no template named 'span' in namespace 'std'

[...]

   24 |     std::span<std::uint64_t> underlying;
      |     ~~~~~^
2 errors generated.
```


What's the problem?
==================================================
