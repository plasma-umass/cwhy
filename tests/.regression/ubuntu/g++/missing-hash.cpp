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
 90    *  Base is _Hashtable, dispatched at compile time via template
 91    *  alias __uset_hashtable.
 92    */
 93   template<typename _Value,
 94 	   typename _Hash = hash<_Value>,
 95 	   typename _Pred = equal_to<_Value>,
 96 	   typename _Alloc = allocator<_Value>>
 97     class unordered_set
 98     {
 99       typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
100       _Hashtable _M_h;
```

File `/usr/include/c++/12/bits/unordered_set.h`:
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

File `/usr/include/c++/12/bits/unordered_set.h`:
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
1572 	       _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::
1573     ~_Hashtable() noexcept
1574     {
1575       // Getting a bucket index from a node shall not throw because it is used
1576       // in methods (erase, swap...) that shall not throw. Need a complete
1577       // type to check this, so do it in the destructor not at class scope.
1578       static_assert(noexcept(declval<const __hash_code_base_access&>()
1579 			._M_bucket_index(declval<const __node_value_type&>(),
1580 					 (std::size_t)0)),
1581 		    "Cache the hash code or qualify your functors involved"
1582 		    " in hash code and bucket index computation with noexcept");
1583 
1584       clear();
1585       _M_deallocate_buckets();
1586     }
1587 
1588   template<typename _Key, typename _Value, typename _Alloc,
1589 	   typename _ExtractKey, typename _Equal,
```

File `/usr/include/c++/12/bits/hashtable.h`:
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
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp: In function ‘void bfs(Node*, std::function<void(Node*)>)’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: use of deleted function ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^~~~~~~
In file included from /usr/include/c++/12/unordered_set:47,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:33:
/usr/include/c++/12/bits/unordered_set.h:135:7: note: ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’ is implicitly deleted because the default definition would be ill-formed:
  135 |       unordered_set() = default;
      |       ^~~~~~~~~~~~~
/usr/include/c++/12/bits/unordered_set.h:135:7: error: use of deleted function ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::_Hashtable() [with _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _Hash = std::hash<std::pair<int, int> >; _RangeHash = std::__detail::_Mod_range_hashing; _Unused = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>]’
In file included from /usr/include/c++/12/unordered_map:46,

[...]

/usr/include/c++/12/bits/hashtable.h:950:21:   required from ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::__ireturn_type std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _Hash = std::hash<std::pair<int, int> >; _RangeHash = std::__detail::_Mod_range_hashing; _Unused = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; __ireturn_type = std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::__ireturn_type]’
/usr/include/c++/12/bits/unordered_set.h:378:23:   required from ‘std::pair<typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator, bool> std::unordered_set<_Value, _Hash, _Pred, _Alloc>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >; typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator = std::__detail::_Insert_base<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::iterator]’
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:51:   required from here
/usr/include/c++/12/bits/hashtable_policy.h:1268:23: error: static assertion failed: hash function must be invocable with an argument of key type
 1268 |         static_assert(__is_invocable<const _Hash&, const _Key&>{},
      |                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/12/bits/hashtable_policy.h:1268:23: note: ‘std::__is_invocable<const std::hash<std::pair<int, int> >&, const std::pair<int, int>&>()’ evaluates to false
/usr/include/c++/12/bits/hashtable_policy.h:1270:25: error: no match for call to ‘(const std::hash<std::pair<int, int> >) (const std::pair<int, int>&)’
 1270 |         return _M_hash()(__k);
      |                ~~~~~~~~~^~~~~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
