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

File `/usr/include/c++/10/bits/unordered_set.h`:
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

File `/usr/include/c++/10/bits/unordered_set.h`:
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

File `/usr/include/c++/10/bits/unordered_set.h`:
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

File `/usr/include/c++/10/bits/hashtable.h`:
```
484       _Hashtable(_Hashtable&&, __node_alloc_type&&,
485 		 false_type /* alloc always equal */);
486 
487 
488     public:
489       // Constructor, destructor, assignment, swap
490       _Hashtable() = default;
491       _Hashtable(size_type __bkt_count_hint,
492 		 const _H1&, const _H2&, const _Hash&,
493 		 const _Equal&, const _ExtractKey&,
```

File `/usr/include/c++/10/bits/hashtable.h`:
```
831     public:
832       // Emplace
833       template<typename... _Args>
834 	__ireturn_type
835 	emplace(_Args&&... __args)
836 	{ return _M_emplace(__unique_keys(), std::forward<_Args>(__args)...); }
837 
838       template<typename... _Args>
839 	iterator
```

File `/usr/include/c++/10/bits/hashtable.h`:
```
1438     _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
1439 	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
1440     ~_Hashtable() noexcept
1441     {
1442       clear();
1443       _M_deallocate_buckets();
1444     }
1445 
1446   template<typename _Key, typename _Value,
1447 	   typename _Alloc, typename _ExtractKey, typename _Equal,
```

File `/usr/include/c++/10/bits/hashtable.h`:
```
1724       _M_emplace(true_type, _Args&&... __args)
1725       -> pair<iterator, bool>
1726       {
1727 	// First build the node to get access to the hash code
1728 	_Scoped_node __node { this, std::forward<_Args>(__args)...  };
1729 	const key_type& __k = this->_M_extract()(__node._M_node->_M_v());
1730 	__hash_code __code = this->_M_hash_code(__k);
1731 	size_type __bkt = _M_bucket_index(__k, __code);
1732 	if (__node_type* __p = _M_find_node(__bkt, __k, __code))
1733 	  // There is already an equivalent node, no insertion
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp: In function ‘void bfs(Node*, std::function<void(Node*)>)’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: use of deleted function ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^~~~~~~
In file included from /usr/include/c++/10/unordered_set:47,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:33:
/usr/include/c++/10/bits/unordered_set.h:135:7: note: ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’ is implicitly deleted because the default definition would be ill-formed:
  135 |       unordered_set() = default;
      |       ^~~~~~~~~~~~~
/usr/include/c++/10/bits/unordered_set.h:135:7: error: use of deleted function ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::_Hashtable() [with _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _H1 = std::hash<std::pair<int, int> >; _H2 = std::__detail::_Mod_range_hashing; _Hash = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>]’

[...]

/usr/include/c++/10/bits/hashtable.h:1730:41:   required from ‘std::pair<typename std::__detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal, _H1, _H2, _Hash, _Traits>::iterator, bool> std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::_M_emplace(std::true_type, _Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _H1 = std::hash<std::pair<int, int> >; _H2 = std::__detail::_Mod_range_hashing; _Hash = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; typename std::__detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal, _H1, _H2, _Hash, _Traits>::iterator = std::__detail::_Hashtable_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Hashtable_traits<true, true, true> >::iterator; std::true_type = std::integral_constant<bool, true>]’
/usr/include/c++/10/bits/hashtable.h:836:21:   required from ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::__ireturn_type std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _H1 = std::hash<std::pair<int, int> >; _H2 = std::__detail::_Mod_range_hashing; _Hash = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::__ireturn_type = std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::__ireturn_type]’
/usr/include/c++/10/bits/unordered_set.h:378:23:   required from ‘std::pair<typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator, bool> std::unordered_set<_Value, _Hash, _Pred, _Alloc>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >; typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator = std::__detail::_Hashtable_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Hashtable_traits<true, true, true> >::iterator]’
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:63:   required from here
/usr/include/c++/10/bits/hashtable_policy.h:1377:16: error: static assertion failed: hash function must be invocable with an argument of key type
 1377 |  static_assert(__is_invocable<const _H1&, const _Key&>{},
      |                ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/10/bits/hashtable_policy.h:1379:16: error: no match for call to ‘(const std::hash<std::pair<int, int> >) (const std::pair<int, int>&)’
 1379 |  return _M_h1()(__k);
      |         ~~~~~~~^~~~~
```


What's the problem?
==================================================
