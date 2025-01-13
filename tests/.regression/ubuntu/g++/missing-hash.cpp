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

File `/usr/include/c++/12/bits/unordered_set.h`:
```
 92    *  Base is _Hashtable, dispatched at compile time via template
 93    *  alias __uset_hashtable.
 94    */
 95   template<typename _Value,
 96 	   typename _Hash = hash<_Value>,
 97 	   typename _Pred = equal_to<_Value>,
 98 	   typename _Alloc = allocator<_Value>>
 99     class unordered_set
100     {
101       typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
102       _Hashtable _M_h;
```

File `/usr/include/c++/12/bits/unordered_set.h`:
```
131       using insert_return_type = typename _Hashtable::insert_return_type;
132 #endif
133 
134       // construct/destroy/copy
135 
136       /// Default constructor.
137       unordered_set() = default;
138 
139       /**
140        *  @brief  Default constructor creates no elements.
```

File `/usr/include/c++/12/bits/unordered_set.h`:
```
374        *
375        *  Insertion requires amortized constant time.
376        */
377       template<typename... _Args>
378 	std::pair<iterator, bool>
379 	emplace(_Args&&... __args)
380 	{ return _M_h.emplace(std::forward<_Args>(__args)...); }
381 
382       /**
383        *  @brief Attempts to insert an element into the %unordered_set.
```

File `/usr/include/c++/12/bits/hashtable.h`:
```
524 	_Hashtable(_InputIterator __first, _InputIterator __last,
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

File `/usr/include/c++/12/bits/hashtable.h`:
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

File `/usr/include/c++/12/bits/hashtable.h`:
```
1587 	       _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::
1588     ~_Hashtable() noexcept
1589     {
1590       // Getting a bucket index from a node shall not throw because it is used
1591       // in methods (erase, swap...) that shall not throw. Need a complete
1592       // type to check this, so do it in the destructor not at class scope.
1593       static_assert(noexcept(declval<const __hash_code_base_access&>()
1594 			._M_bucket_index(declval<const __node_value_type&>(),
1595 					 (std::size_t)0)),
1596 		    "Cache the hash code or qualify your functors involved"
1597 		    " in hash code and bucket index computation with noexcept");
1598 
1599       clear();
1600       _M_deallocate_buckets();
1601     }
1602 
1603   template<typename _Key, typename _Value, typename _Alloc,
1604 	   typename _ExtractKey, typename _Equal,
```

File `/usr/include/c++/12/bits/hashtable.h`:
```
2082 	    for (auto __it = begin(); __it != end(); ++__it)
2083 	      if (this->_M_key_equals(__k, *__it._M_cur))
2084 		// There is already an equivalent node, no insertion
2085 		return { __it, false };
2086 	  }
2087 
2088 	__hash_code __code = this->_M_hash_code(__k);
2089 	size_type __bkt = _M_bucket_index(__code);
2090 	if (size() > __small_size_threshold())
2091 	  if (__node_ptr __p = _M_find_node(__bkt, __k, __code))
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp: In function ‘void bfs(Node*, std::function<void(Node*)>)’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: use of deleted function ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^~~~~~~
In file included from /usr/include/c++/12/unordered_set:47,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:33:
/usr/include/c++/12/bits/unordered_set.h:137:7: note: ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’ is implicitly deleted because the default definition would be ill-formed:
  137 |       unordered_set() = default;
      |       ^~~~~~~~~~~~~
/usr/include/c++/12/bits/unordered_set.h:137:7: error: use of deleted function ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::_Hashtable() [with _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _Hash = std::hash<std::pair<int, int> >; _RangeHash = std::__detail::_Mod_range_hashing; _Unused = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>]’
In file included from /usr/include/c++/12/unordered_map:46,

[...]

/usr/include/c++/12/bits/hashtable.h:950:21:   required from ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::__ireturn_type std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _Hash = std::hash<std::pair<int, int> >; _RangeHash = std::__detail::_Mod_range_hashing; _Unused = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; __ireturn_type = std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::__ireturn_type]’
/usr/include/c++/12/bits/unordered_set.h:380:23:   required from ‘std::pair<typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator, bool> std::unordered_set<_Value, _Hash, _Pred, _Alloc>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >; typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator = std::__detail::_Insert_base<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::iterator]’
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:51:   required from here
/usr/include/c++/12/bits/hashtable_policy.h:1272:23: error: static assertion failed: hash function must be invocable with an argument of key type
 1272 |         static_assert(__is_invocable<const _Hash&, const _Key&>{},
      |                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/12/bits/hashtable_policy.h:1272:23: note: ‘std::__is_invocable<const std::hash<std::pair<int, int> >&, const std::pair<int, int>&>()’ evaluates to false
/usr/include/c++/12/bits/hashtable_policy.h:1274:25: error: no match for call to ‘(const std::hash<std::pair<int, int> >) (const std::pair<int, int>&)’
 1274 |         return _M_hash()(__k);
      |                ~~~~~~~~~^~~~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
