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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h`:
```
1269        *  element at the end of the %vector and assigns the given data
1270        *  to it.  Due to the nature of a %vector this operation can be
1271        *  done in constant time if the %vector has preallocated space
1272        *  available.
1273        */
1274       _GLIBCXX20_CONSTEXPR
1275       void
1276       push_back(const value_type& __x)
1277       {
1278 	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
1279 	  {
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h`:
```
1287 	  _M_realloc_insert(end(), __x);
1288       }
1289 
1290 #if __cplusplus >= 201103L
1291       _GLIBCXX20_CONSTEXPR
1292       void
1293       push_back(value_type&& __x)
1294       { emplace_back(std::move(__x)); }
1295 
1296       template<typename... _Args>
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/push-back-pointer.cpp:24:7: error: no matching member function for call to 'push_back'
   24 |     v.push_back(pointer);
      |     ~~^~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h:1276:7: note: candidate function not viable: no known conversion from 'int *' to 'const value_type' (aka 'const int') for 1st argument; dereference the argument with *
 1276 |       push_back(const value_type& __x)

[...]

      |       ^         ~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h:1293:7: note: candidate function not viable: no known conversion from 'int *' to 'value_type' (aka 'int') for 1st argument; dereference the argument with *
 1293 |       push_back(value_type&& __x)
      |       ^         ~~~~~~~~~~~~~~~~
1 error generated.
```


What's the problem?
==================================================
