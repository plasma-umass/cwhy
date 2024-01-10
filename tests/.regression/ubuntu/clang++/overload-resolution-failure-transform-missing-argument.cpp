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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h`:
```
4291    *
4292    *  @p unary_op must not alter its argument.
4293   */
4294   template<typename _InputIterator, typename _OutputIterator,
4295 	   typename _UnaryOperation>
4296     _GLIBCXX20_CONSTEXPR
4297     _OutputIterator
4298     transform(_InputIterator __first, _InputIterator __last,
4299 	      _OutputIterator __result, _UnaryOperation __unary_op)
4300     {
4301       // concept requirements
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h`:
```
4330    *  @p binary_op must not alter either of its arguments.
4331   */
4332   template<typename _InputIterator1, typename _InputIterator2,
4333 	   typename _OutputIterator, typename _BinaryOperation>
4334     _GLIBCXX20_CONSTEXPR
4335     _OutputIterator
4336     transform(_InputIterator1 __first1, _InputIterator1 __last1,
4337 	      _InputIterator2 __first2, _OutputIterator __result,
4338 	      _BinaryOperation __binary_op)
4339     {
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/pstl/glue_algorithm_defs.h`:
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
/home/runner/work/cwhy/cwhy/tests/c++/overload-resolution-failure-transform-missing-argument.cpp:41:5: error: no matching function for call to 'transform'
   41 |     std::transform(v.begin(), v.end(), [](int i) { return i * i; });
      |     ^~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:4298:5: note: candidate function template not viable: requires 4 arguments, but 3 were provided
 4298 |     transform(_InputIterator __first, _InputIterator __last,
      |     ^         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 4299 |               _OutputIterator __result, _UnaryOperation __unary_op)
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:4336:5: note: candidate function template not viable: requires 5 arguments, but 3 were provided
 4336 |     transform(_InputIterator1 __first1, _InputIterator1 __last1,
      |     ^         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 4337 |               _InputIterator2 __first2, _OutputIterator __result,
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[...]

 4338 |               _BinaryOperation __binary_op)
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/pstl/glue_algorithm_defs.h:156:1: note: candidate function template not viable: requires 5 arguments, but 3 were provided
  156 | transform(_ExecutionPolicy&& __exec, _ForwardIterator1 __first, _ForwardIterator1 __last, _ForwardIterator2 __result,
      | ^         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  157 |           _UnaryOperation __op);
      |           ~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/pstl/glue_algorithm_defs.h:162:1: note: candidate function template not viable: requires 6 arguments, but 3 were provided
  162 | transform(_ExecutionPolicy&& __exec, _ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2,
      | ^         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  163 |           _ForwardIterator __result, _BinaryOperation __op);
      |           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
