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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h`:
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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h`:
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
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:7: error: no matching member function for call to 'push_back'
   24 |     v.push_back(pointer);
      |     ~~^~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:1278:7: note: candidate function not viable: no known conversion from 'int *' to 'const value_type' (aka 'const int') for 1st argument; dereference the argument with *
 1278 |       push_back(const value_type& __x)

[...]

      |       ^         ~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:1295:7: note: candidate function not viable: no known conversion from 'int *' to 'value_type' (aka 'int') for 1st argument; dereference the argument with *
 1295 |       push_back(value_type&& __x)
      |       ^         ~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
