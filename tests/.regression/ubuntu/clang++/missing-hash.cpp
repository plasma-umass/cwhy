===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp`:
```
37 struct Node {
38     const std::pair<int, int> position;
39     std::vector<Node*> neighbors;
40 };
41 
42 void bfs(Node* start, std::function<void(Node*)> f) {
43     std::unordered_set<std::pair<int, int>> visited;
44     std::queue<Node*> queue;
45     queue.push(start);
46 
47     while (!queue.empty()) {
48         auto* n = queue.front();
49         queue.pop();
50         const auto [_, inserted] = visited.emplace(n->position);
51         if (inserted) {
52             f(n);
53             for (auto* neighbor : n->neighbors) {
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_set.h`:
```
100   template<typename _Value,
101 	   typename _Hash = hash<_Value>,
102 	   typename _Pred = equal_to<_Value>,
103 	   typename _Alloc = allocator<_Value>>
104     class unordered_set
105     {
106       typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
107       _Hashtable _M_h;
108 
109     public:
110       // typedefs:
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_set.h`:
```
136       using insert_return_type = typename _Hashtable::insert_return_type;
137 #endif
138 
139       // construct/destroy/copy
140 
141       /// Default constructor.
142       unordered_set() = default;
143 
144       /**
145        *  @brief  Default constructor creates no elements.
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_set.h`:
```
379        *
380        *  Insertion requires amortized constant time.
381        */
382       template<typename... _Args>
383 	std::pair<iterator, bool>
384 	emplace(_Args&&... __args)
385 	{ return _M_h.emplace(std::forward<_Args>(__args)...); }
386 
387       /**
388        *  @brief Attempts to insert an element into the %unordered_set.
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h`:
```
179    *    - __detail::_Equality
180    */
181   template<typename _Key, typename _Value, typename _Alloc,
182 	   typename _ExtractKey, typename _Equal,
183 	   typename _Hash, typename _RangeHash, typename _Unused,
184 	   typename _RehashPolicy, typename _Traits>
185     class _Hashtable
186     : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
187 				       _Hash, _RangeHash, _Unused, _Traits>,
188       public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,
189 				 _Hash, _RangeHash, _Unused,
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h`:
```
533 		   size_type __bkt_count_hint,
534 		   const _Hash&, const _Equal&, const allocator_type&,
535 		   false_type __uks);
536 
537     public:
538       // Constructor, destructor, assignment, swap
539       _Hashtable() = default;
540 
541       _Hashtable(const _Hashtable&);
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h`:
```
 996     public:
 997       // Emplace
 998       template<typename... _Args>
 999 	__ireturn_type
1000 	emplace(_Args&&... __args)
1001 	{ return _M_emplace(__unique_keys{}, std::forward<_Args>(__args)...); }
1002 
1003       template<typename... _Args>
1004 	iterator
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h`:
```
2148 	    for (auto __it = _M_begin(); __it; __it = __it->_M_next())
2149 	      if (this->_M_key_equals(__k, *__it))
2150 		// There is already an equivalent node, no insertion
2151 		return { iterator(__it), false };
2152 	  }
2153 
2154 	__hash_code __code = this->_M_hash_code(__k);
2155 	size_type __bkt = _M_bucket_index(__code);
2156 	if (__size > __small_size_threshold())
2157 	  if (__node_ptr __p = _M_find_node(__bkt, __k, __code))
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: call to implicitly-deleted default constructor of 'std::unordered_set<std::pair<int, int>>'
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_set.h:142:7: note: explicitly defaulted function was implicitly deleted here
  142 |       unordered_set() = default;
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_set.h:107:18: note: default constructor of 'unordered_set<std::pair<int, int>>' is implicitly deleted because field '_M_h' has a deleted default constructor
  107 |       _Hashtable _M_h;
      |                  ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h:539:7: note: explicitly defaulted function was implicitly deleted here
  539 |       _Hashtable() = default;
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h:186:7: note: default constructor of '_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>' is implicitly deleted because base class '__detail::_Hashtable_base<pair<int, int>, pair<int, int>, _Identity, equal_to<pair<int, int>>, hash<pair<int, int>>, _Mod_range_hashing, _Default_ranged_hash, _Hashtable_traits<true, true, true>>' has a deleted default constructor
  186 |     : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable_policy.h:1726:7: note: explicitly defaulted function was implicitly deleted here
 1726 |       _Hashtable_base() = default;
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable_policy.h:1684:7: note: default constructor of '_Hashtable_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Hashtable_traits<true, true, true>>' is implicitly deleted because base class '_Hash_code_base<pair<int, int>, pair<int, int>, _Identity, hash<pair<int, int>>, _Mod_range_hashing, _Default_ranged_hash, _Hashtable_traits<true, true, true>::__hash_cached::value>' has a deleted default constructor
 1684 |     : public _Hash_code_base<_Key, _Value, _ExtractKey, _Hash, _RangeHash,
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable_policy.h:1326:7: note: explicitly defaulted function was implicitly deleted here
 1326 |       _Hash_code_base() = default;
      |       ^

[...]

/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable_policy.h:1333:16: error: static assertion failed due to requirement 'std::__is_invocable<const std::hash<std::pair<int, int>> &, const std::pair<int, int> &>{}': hash function must be invocable with an argument of key type
 1333 |         static_assert(__is_invocable<const _Hash&, const _Key&>{},
      |                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h:2154:29: note: in instantiation of member function 'std::__detail::_Hash_code_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, true>::_M_hash_code' requested here
 2154 |         __hash_code __code = this->_M_hash_code(__k);
      |                                    ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h:1001:11: note: in instantiation of function template specialization 'std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>::_M_emplace<const std::pair<int, int> &>' requested here
 1001 |         { return _M_emplace(__unique_keys{}, std::forward<_Args>(__args)...); }
      |                  ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_set.h:385:16: note: in instantiation of function template specialization 'std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>::emplace<const std::pair<int, int> &>' requested here
  385 |         { return _M_h.emplace(std::forward<_Args>(__args)...); }
      |                       ^
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:44: note: in instantiation of function template specialization 'std::unordered_set<std::pair<int, int>>::emplace<const std::pair<int, int> &>' requested here
   50 |         const auto [_, inserted] = visited.emplace(n->position);
      |                                            ^
In file included from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:31:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/functional:63:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/unordered_map:41:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/unordered_map.h:33:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable.h:35:
/usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/hashtable_policy.h:1335:9: error: type 'const std::hash<std::pair<int, int>>' does not provide a call operator
 1335 |         return _M_hash()(__k);
      |                ^~~~~~~~~
3 errors generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
