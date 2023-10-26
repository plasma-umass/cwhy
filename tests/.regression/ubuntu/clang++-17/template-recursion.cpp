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
```

File `/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp`:
```
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
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:12:17: error: no template named 'span' in namespace 'std'
   12 |     Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
      |            ~~~~~^
/home/runner/work/cwhy/cwhy/tests/c++/template-recursion.cpp:25:10: error: no template named 'span' in namespace 'std'

[...]

   25 |     std::span<std::uint64_t> underlying;
      |     ~~~~~^
2 errors generated.
```


What's the problem?
==================================================
