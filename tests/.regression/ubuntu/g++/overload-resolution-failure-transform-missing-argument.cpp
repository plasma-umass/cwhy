===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp`:
```
34 iterator that appends new elements to the `v_out` vector.
35 */
36 #include <algorithm>
37 #include <vector>
38 
39 int main() {
40     std::vector<int> v;
41     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
42 }
```

File `/usr/include/c++/12/bits/stl_algo.h`:
```
4245    *
4246    *  @p unary_op must not alter its argument.
4247   */
4248   template<typename _InputIterator, typename _OutputIterator,
4249 	   typename _UnaryOperation>
4250     _GLIBCXX20_CONSTEXPR
4251     _OutputIterator
4252     transform(_InputIterator __first, _InputIterator __last,
4253 	      _OutputIterator __result, _UnaryOperation __unary_op)
4254     {
4255       // concept requirements
```

File `/usr/include/c++/12/bits/stl_algo.h`:
```
4284    *  @p binary_op must not alter either of its arguments.
4285   */
4286   template<typename _InputIterator1, typename _InputIterator2,
4287 	   typename _OutputIterator, typename _BinaryOperation>
4288     _GLIBCXX20_CONSTEXPR
4289     _OutputIterator
4290     transform(_InputIterator1 __first1, _InputIterator1 __last1,
4291 	      _InputIterator2 __first2, _OutputIterator __result,
4292 	      _BinaryOperation __binary_op)
4293     {
```

File `/usr/include/c++/12/pstl/glue_algorithm_defs.h`:
```
149 swap_ranges(_ExecutionPolicy&& __exec, _ForwardIterator1 __first1, _ForwardIterator1 __last1,
150             _ForwardIterator2 __first2);
151 
152 // [alg.transform]
153 
154 template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _UnaryOperation>
155 __pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator2>
156 transform(_ExecutionPolicy&& __exec, _ForwardIterator1 __first, _ForwardIterator1 __last, _ForwardIterator2 __result,
157           _UnaryOperation __op);
158 
159 template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
160           class _BinaryOperation>
161 __pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator>
162 transform(_ExecutionPolicy&& __exec, _ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2,
163           _ForwardIterator __result, _BinaryOperation __op);
164 
165 // [alg.replace]
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:41:19: error: no matching function for call to ‘transform(std::vector<int>::iterator, std::vector<int>::iterator, main()::<lambda(int)>)’
   41 |     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
      |     ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /usr/include/c++/12/algorithm:61,
                 from /home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:36:
/usr/include/c++/12/bits/stl_algo.h:4252:5: note: candidate: ‘template<class _IIter, class _OIter, class _UnaryOperation> constexpr _OIter std::transform(_IIter, _IIter, _OIter, _UnaryOperation)’
 4252 |     transform(_InputIterator __first, _InputIterator __last,
      |     ^~~~~~~~~
/usr/include/c++/12/bits/stl_algo.h:4252:5: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:41:19: note:   candidate expects 4 arguments, 3 provided
   41 |     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
      |     ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/12/bits/stl_algo.h:4290:5: note: candidate: ‘template<class _IIter1, class _IIter2, class _OIter, class _BinaryOperation> constexpr _OIter std::transform(_IIter1, _IIter1, _IIter2, _OIter, _BinaryOperation)’
 4290 |     transform(_InputIterator1 __first1, _InputIterator1 __last1,
      |     ^~~~~~~~~
/usr/include/c++/12/bits/stl_algo.h:4290:5: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:41:19: note:   candidate expects 5 arguments, 3 provided

[...]

   41 |     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
      |     ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /usr/include/c++/12/algorithm:73:
/usr/include/c++/12/pstl/glue_algorithm_defs.h:156:1: note: candidate: ‘template<class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _UnaryOperation> __pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator2> std::transform(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _UnaryOperation)’
  156 | transform(_ExecutionPolicy&& __exec, _ForwardIterator1 __first, _ForwardIterator1 __last, _ForwardIterator2 __result,
      | ^~~~~~~~~
/usr/include/c++/12/pstl/glue_algorithm_defs.h:156:1: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:41:19: note:   deduced conflicting types for parameter ‘_ForwardIterator1’ (‘__gnu_cxx::__normal_iterator<int*, std::vector<int> >’ and ‘main()::<lambda(int)>’)
   41 |     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
      |     ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/12/pstl/glue_algorithm_defs.h:162:1: note: candidate: ‘template<class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator, class _BinaryOperation> __pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator2> std::transform(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator, _BinaryOperation)’
  162 | transform(_ExecutionPolicy&& __exec, _ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2,
      | ^~~~~~~~~
/usr/include/c++/12/pstl/glue_algorithm_defs.h:162:1: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:41:19: note:   deduced conflicting types for parameter ‘_ForwardIterator1’ (‘__gnu_cxx::__normal_iterator<int*, std::vector<int> >’ and ‘main()::<lambda(int)>’)
   41 |     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
      |     ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
