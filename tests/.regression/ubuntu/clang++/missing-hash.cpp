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

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unordered_set.h`:
```
 93   template<typename _Value,
 94 	   typename _Hash = hash<_Value>,
 95 	   typename _Pred = equal_to<_Value>,
 96 	   typename _Alloc = allocator<_Value>>
 97     class unordered_set
 98     {
 99       typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
100       _Hashtable _M_h;
101 
102     public:
103       // typedefs:
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unordered_set.h`:
```
129       using insert_return_type = typename _Hashtable::insert_return_type;
130 #endif
131 
132       // construct/destroy/copy
133 
134       /// Default constructor.
135       unordered_set() = default;
136 
137       /**
138        *  @brief  Default constructor creates no elements.
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unordered_set.h`:
```
372        *
373        *  Insertion requires amortized constant time.
374        */
375       template<typename... _Args>
376 	std::pair<iterator, bool>
377 	emplace(_Args&&... __args)
378 	{ return _M_h.emplace(std::forward<_Args>(__args)...); }
379 
380       /**
381        *  @brief Attempts to insert an element into the %unordered_set.
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h`:
```
176    *    - __detail::_Equality
177    */
178   template<typename _Key, typename _Value, typename _Alloc,
179 	   typename _ExtractKey, typename _Equal,
180 	   typename _Hash, typename _RangeHash, typename _Unused,
181 	   typename _RehashPolicy, typename _Traits>
182     class _Hashtable
183     : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
184 				       _Hash, _RangeHash, _Unused, _Traits>,
185       public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,
186 				 _Hash, _RangeHash, _Unused,
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h`:
```
525 		   size_type __bkt_count_hint,
526 		   const _Hash&, const _Equal&, const allocator_type&,
527 		   false_type __uks);
528 
529     public:
530       // Constructor, destructor, assignment, swap
531       _Hashtable() = default;
532 
533       _Hashtable(const _Hashtable&);
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h`:
```
945     public:
946       // Emplace
947       template<typename... _Args>
948 	__ireturn_type
949 	emplace(_Args&&... __args)
950 	{ return _M_emplace(__unique_keys{}, std::forward<_Args>(__args)...); }
951 
952       template<typename... _Args>
953 	iterator
```

File `/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h`:
```
2067 	    for (auto __it = begin(); __it != end(); ++__it)
2068 	      if (this->_M_key_equals(__k, *__it._M_cur))
2069 		// There is already an equivalent node, no insertion
2070 		return { __it, false };
2071 	  }
2072 
2073 	__hash_code __code = this->_M_hash_code(__k);
2074 	size_type __bkt = _M_bucket_index(__code);
2075 	if (size() > __small_size_threshold())
2076 	  if (__node_ptr __p = _M_find_node(__bkt, __k, __code))
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: call to implicitly-deleted default constructor of 'std::unordered_set<std::pair<int, int>>'
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unordered_set.h:135:7: note: explicitly defaulted function was implicitly deleted here
  135 |       unordered_set() = default;
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unordered_set.h:100:18: note: default constructor of 'unordered_set<std::pair<int, int>>' is implicitly deleted because field '_M_h' has a deleted default constructor
  100 |       _Hashtable _M_h;
      |                  ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h:531:7: note: explicitly defaulted function was implicitly deleted here
  531 |       _Hashtable() = default;
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h:183:7: note: default constructor of '_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>' is implicitly deleted because base class '__detail::_Hashtable_base<pair<int, int>, pair<int, int>, _Identity, equal_to<pair<int, int>>, hash<pair<int, int>>, _Mod_range_hashing, _Default_ranged_hash, _Hashtable_traits<true, true, true>>' has a deleted default constructor
  183 |     : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable_policy.h:1674:7: note: explicitly defaulted function was implicitly deleted here
 1674 |       _Hashtable_base() = default;
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable_policy.h:1632:7: note: default constructor of '_Hashtable_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Hashtable_traits<true, true, true>>' is implicitly deleted because base class '_Hash_code_base<pair<int, int>, pair<int, int>, _Identity, hash<pair<int, int>>, _Mod_range_hashing, _Default_ranged_hash, _Hashtable_traits<true, true, true>::__hash_cached::value>' has a deleted default constructor
 1632 |     : public _Hash_code_base<_Key, _Value, _ExtractKey, _Hash, _RangeHash,
      |       ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable_policy.h:1261:7: note: explicitly defaulted function was implicitly deleted here
 1261 |       _Hash_code_base() = default;
      |       ^

[...]

In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h:35:
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable_policy.h:1268:16: error: static assertion failed due to requirement 'std::__is_invocable<const std::hash<std::pair<int, int>> &, const std::pair<int, int> &>{}': hash function must be invocable with an argument of key type
 1268 |         static_assert(__is_invocable<const _Hash&, const _Key&>{},
      |                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h:2073:29: note: in instantiation of member function 'std::__detail::_Hash_code_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, true>::_M_hash_code' requested here
 2073 |         __hash_code __code = this->_M_hash_code(__k);
      |                                    ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h:950:11: note: in instantiation of function template specialization 'std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>::_M_emplace<const std::pair<int, int> &>' requested here
  950 |         { return _M_emplace(__unique_keys{}, std::forward<_Args>(__args)...); }
      |                  ^
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unordered_set.h:378:16: note: in instantiation of function template specialization 'std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>::emplace<const std::pair<int, int> &>' requested here
  378 |         { return _M_h.emplace(std::forward<_Args>(__args)...); }
      |                       ^
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:44: note: in instantiation of function template specialization 'std::unordered_set<std::pair<int, int>>::emplace<const std::pair<int, int> &>' requested here
   50 |         const auto [_, inserted] = visited.emplace(n->position);
      |                                            ^
In file included from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:31:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/functional:61:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/unordered_map:46:
In file included from /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable.h:35:
/usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/hashtable_policy.h:1270:9: error: type 'const std::hash<std::pair<int, int>>' does not provide a call operator
 1270 |         return _M_hash()(__k);
      |                ^~~~~~~~~
3 errors generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
