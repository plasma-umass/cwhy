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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/stl_vector.h`:
```
1276        *  element at the end of the %vector and assigns the given data
1277        *  to it.  Due to the nature of a %vector this operation can be
1278        *  done in constant time if the %vector has preallocated space
1279        *  available.
1280        */
1281       _GLIBCXX20_CONSTEXPR
1282       void
1283       push_back(const value_type& __x)
1284       {
1285 	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
1286 	  {
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/stl_vector.h`:
```
1294 	  _M_realloc_append(__x);
1295       }
1296 
1297 #if __cplusplus >= 201103L
1298       _GLIBCXX20_CONSTEXPR
1299       void
1300       push_back(value_type&& __x)
1301       { emplace_back(std::move(__x)); }
1302 
1303       template<typename... _Args>
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:7: error: no matching member function for call to 'push_back'
   24 |     v.push_back(pointer);
      |     ~~^~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/stl_vector.h:1283:7: note: candidate function not viable: no known conversion from 'int *' to 'const value_type' (aka 'const int') for 1st argument; dereference the argument with *
 1283 |       push_back(const value_type& __x)
      |       ^         ~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/stl_vector.h:1300:7: note: candidate function not viable: no known conversion from 'int *' to 'value_type' (aka 'int') for 1st argument; dereference the argument with *
 1300 |       push_back(value_type&& __x)
      |       ^         ~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
