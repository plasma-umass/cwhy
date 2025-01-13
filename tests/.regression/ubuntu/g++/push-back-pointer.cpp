===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp`:
```
17 */
18 #include <vector>
19 
20 int main() {
21     std::vector<int> v;
22     int value = 10;
23     int* pointer = &value;
24     v.push_back(pointer);
25 }
```

File `/usr/include/c++/12/bits/stl_vector.h`:
```
1271        *  element at the end of the %vector and assigns the given data
1272        *  to it.  Due to the nature of a %vector this operation can be
1273        *  done in constant time if the %vector has preallocated space
1274        *  available.
1275        */
1276       _GLIBCXX20_CONSTEXPR
1277       void
1278       push_back(const value_type& __x)
1279       {
1280 	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
1281 	  {
```

File `/usr/include/c++/12/bits/stl_vector.h`:
```
1289 	  _M_realloc_insert(end(), __x);
1290       }
1291 
1292 #if __cplusplus >= 201103L
1293       _GLIBCXX20_CONSTEXPR
1294       void
1295       push_back(value_type&& __x)
1296       { emplace_back(std::move(__x)); }
1297 
1298       template<typename... _Args>
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:16: error: no matching function for call to ‘push_back(int*&)’
   24 |     v.push_back(pointer);
      |     ~~~~~~~~~~~^~~~~~~~~
In file included from /usr/include/c++/12/vector:64,
                 from /home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:18:
/usr/include/c++/12/bits/stl_vector.h:1278:7: note: candidate: ‘constexpr void std::vector<_Tp, _Alloc>::push_back(const value_type&) [with _Tp = int; _Alloc = std::allocator<int>; value_type = int]’ (near match)
 1278 |       push_back(const value_type& __x)
      |       ^~~~~~~~~
/usr/include/c++/12/bits/stl_vector.h:1278:7: note:   conversion of argument 1 would be ill-formed:
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:17: error: invalid conversion from ‘int*’ to ‘std::vector<int>::value_type’ {aka ‘int’} [-fpermissive]
   24 |     v.push_back(pointer);
      |                 ^~~~~~~
      |                 |
      |                 int*
/usr/include/c++/12/bits/stl_vector.h:1295:7: note: candidate: ‘constexpr void std::vector<_Tp, _Alloc>::push_back(value_type&&) [with _Tp = int; _Alloc = std::allocator<int>; value_type = int]’ (near match)
 1295 |       push_back(value_type&& __x)
      |       ^~~~~~~~~
/usr/include/c++/12/bits/stl_vector.h:1295:7: note:   conversion of argument 1 would be ill-formed:
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:17: error: invalid conversion from ‘int*’ to ‘std::vector<int>::value_type’ {aka ‘int’} [-fpermissive]
   24 |     v.push_back(pointer);
      |                 ^~~~~~~
      |                 |
      |                 int*
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
