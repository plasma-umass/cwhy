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

File `/usr/include/c++/11/bits/stl_vector.h`:
```
1180        *  This is a typical stack operation.  The function creates an
1181        *  element at the end of the %vector and assigns the given data
1182        *  to it.  Due to the nature of a %vector this operation can be
1183        *  done in constant time if the %vector has preallocated space
1184        *  available.
1185        */
1186       void
1187       push_back(const value_type& __x)
1188       {
1189 	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
1190 	  {
```

File `/usr/include/c++/11/bits/stl_vector.h`:
```
1197 	else
1198 	  _M_realloc_insert(end(), __x);
1199       }
1200 
1201 #if __cplusplus >= 201103L
1202       void
1203       push_back(value_type&& __x)
1204       { emplace_back(std::move(__x)); }
1205 
1206       template<typename... _Args>
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp: In function ‘int main()’:
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:16: error: no matching function for call to ‘push_back(int*&)’
   24 |     v.push_back(pointer);
      |     ~~~~~~~~~~~^~~~~~~~~
In file included from /usr/include/c++/11/vector:67,
                 from /home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:18:
/usr/include/c++/11/bits/stl_vector.h:1187:7: note: candidate: ‘void std::vector<_Tp, _Alloc>::push_back(const value_type&) [with _Tp = int; _Alloc = std::allocator<int>; std::vector<_Tp, _Alloc>::value_type = int]’ (near match)
 1187 |       push_back(const value_type& __x)
      |       ^~~~~~~~~
/usr/include/c++/11/bits/stl_vector.h:1187:7: note:   conversion of argument 1 would be ill-formed:
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:17: error: invalid conversion from ‘int*’ to ‘std::vector<int>::value_type’ {aka ‘int’} [-fpermissive]
   24 |     v.push_back(pointer);
      |                 ^~~~~~~

[...]

      |                 |
      |                 int*
In file included from /usr/include/c++/11/vector:67,
                 from /home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:18:
/usr/include/c++/11/bits/stl_vector.h:1203:7: note: candidate: ‘void std::vector<_Tp, _Alloc>::push_back(std::vector<_Tp, _Alloc>::value_type&&) [with _Tp = int; _Alloc = std::allocator<int>; std::vector<_Tp, _Alloc>::value_type = int]’ (near match)
 1203 |       push_back(value_type&& __x)
      |       ^~~~~~~~~
/usr/include/c++/11/bits/stl_vector.h:1203:7: note:   conversion of argument 1 would be ill-formed:
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:17: error: invalid conversion from ‘int*’ to ‘std::vector<int>::value_type’ {aka ‘int’} [-fpermissive]
   24 |     v.push_back(pointer);
      |                 ^~~~~~~
      |                 |
      |                 int*
```


What's the problem?
==================================================
