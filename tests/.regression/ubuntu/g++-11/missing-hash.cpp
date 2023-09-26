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

File `/usr/include/c++/11/bits/unordered_set.h`:
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

File `/usr/include/c++/11/bits/unordered_set.h`:
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

File `/usr/include/c++/11/bits/unordered_set.h`:
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

File `/usr/include/c++/11/bits/hashtable.h`:
```
521 	_Hashtable(_InputIterator __first, _InputIterator __last,
522 		   size_type __bkt_count_hint,
523 		   const _Hash&, const _Equal&, const allocator_type&,
524 		   false_type __uks);
525 
526     public:
527       // Constructor, destructor, assignment, swap
528       _Hashtable() = default;
529 
530       _Hashtable(const _Hashtable&);
```

File `/usr/include/c++/11/bits/hashtable.h`:
```
910     public:
911       // Emplace
912       template<typename... _Args>
913 	__ireturn_type
914 	emplace(_Args&&... __args)
915 	{ return _M_emplace(__unique_keys{}, std::forward<_Args>(__args)...); }
916 
917       template<typename... _Args>
918 	iterator
```

File `/usr/include/c++/11/bits/hashtable.h`:
```
1528     _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
1529 	       _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::
1530     ~_Hashtable() noexcept
1531     {
1532       clear();
1533       _M_deallocate_buckets();
1534     }
1535 
1536   template<typename _Key, typename _Value, typename _Alloc,
1537 	   typename _ExtractKey, typename _Equal,
```

File `/usr/include/c++/11/bits/hashtable.h`:
```
1962       _M_emplace(true_type /* __uks */, _Args&&... __args)
1963       -> pair<iterator, bool>
1964       {
1965 	// First build the node to get access to the hash code
1966 	_Scoped_node __node { this, std::forward<_Args>(__args)...  };
1967 	const key_type& __k = _ExtractKey{}(__node._M_node->_M_v());
1968 	__hash_code __code = this->_M_hash_code(__k);
1969 	size_type __bkt = _M_bucket_index(__code);
1970 	if (__node_ptr __p = _M_find_node(__bkt, __k, __code))
1971 	  // There is already an equivalent node, no insertion
```

File `/usr/include/c++/11/bits/hashtable_policy.h`:
```
1117    */
1118   template<int _Nm, typename _Tp,
1119 	   bool __use_ebo = !__is_final(_Tp) && __is_empty(_Tp)>
1120     struct _Hashtable_ebo_helper;
1121 
1122   /// Specialization using EBO.
1123   template<int _Nm, typename _Tp>
1124     struct _Hashtable_ebo_helper<_Nm, _Tp, true>
1125     : private _Tp
1126     {
1127       _Hashtable_ebo_helper() noexcept(noexcept(_Tp())) : _Tp() { }
1128 
1129       template<typename _OtherTp>
1130 	_Hashtable_ebo_helper(_OtherTp&& __tp)
```

File `/usr/include/c++/11/bits/hashtable_policy.h`:
```
1182    *  to implement local_iterator and const_local_iterator. As with
1183    *  any iterator type we prefer to make it as small as possible.
1184    */
1185   template<typename _Key, typename _Value, typename _ExtractKey,
1186 	   typename _Hash, typename _RangeHash, typename _Unused,
1187 	   bool __cache_hash_code>
1188     struct _Hash_code_base
1189     : private _Hashtable_ebo_helper<1, _Hash>
1190     {
1191     private:
```

File `/usr/include/c++/11/bits/hashtable_policy.h`:
```
1205     protected:
1206       typedef std::size_t 				__hash_code;
1207 
1208       // We need the default constructor for the local iterators and _Hashtable
1209       // default constructor.
1210       _Hash_code_base() = default;
1211 
1212       _Hash_code_base(const _Hash& __hash) : __ebo_hash(__hash) { }
1213 
1214       __hash_code
1215       _M_hash_code(const _Key& __k) const
1216       {
1217 	static_assert(__is_invocable<const _Hash&, const _Key&>{},
1218 	    "hash function must be invocable with an argument of key type");
1219 	return _M_hash()(__k);
1220       }
1221 
1222       template<typename _Kt>
```

File `/usr/include/c++/11/bits/hashtable_policy.h`:
```
1555    *    - __detail::_Hash_code_base
1556    *    - __detail::_Hashtable_ebo_helper
1557    */
1558   template<typename _Key, typename _Value, typename _ExtractKey,
1559 	   typename _Equal, typename _Hash, typename _RangeHash,
1560 	   typename _Unused, typename _Traits>
1561     struct _Hashtable_base
1562     : public _Hash_code_base<_Key, _Value, _ExtractKey, _Hash, _RangeHash,
1563 			     _Unused, _Traits::__hash_cached::value>,
1564       private _Hashtable_ebo_helper<0, _Equal>
```

File `/usr/include/c++/11/bits/hashtable_policy.h`:
```
1598       static bool
1599       _S_node_equals(const _Hash_node_code_cache<true>& __lhn,
1600 		     const _Hash_node_code_cache<true>& __rhn)
1601       { return __lhn._M_hash_code == __rhn._M_hash_code; }
1602 
1603     protected:
1604       _Hashtable_base() = default;
1605 
1606       _Hashtable_base(const _Hash& __hash, const _Equal& __eq)
1607       : __hash_code_base(__hash), _EqualEBO(__eq)
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp: In function ‘void bfs(Node*, std::function<void(Node*)>)’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: use of deleted function ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^~~~~~~
In file included from /usr/include/c++/11/unordered_set:47,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:33:
/usr/include/c++/11/bits/unordered_set.h:135:7: note: ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’ is implicitly deleted because the default definition would be ill-formed:
  135 |       unordered_set() = default;
      |       ^~~~~~~~~~~~~
/usr/include/c++/11/bits/unordered_set.h:135:7: error: use of deleted function ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::_Hashtable() [with _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _Hash = std::hash<std::pair<int, int> >; _RangeHash = std::__detail::_Mod_range_hashing; _Unused = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>]’
In file included from /usr/include/c++/11/unordered_map:46,

[...]

/usr/include/c++/11/bits/hashtable.h:915:21:   required from ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::__ireturn_type std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _Hash = std::hash<std::pair<int, int> >; _RangeHash = std::__detail::_Mod_range_hashing; _Unused = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _Hash, _RangeHash, _Unused, _RehashPolicy, _Traits>::__ireturn_type = std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::__ireturn_type]’
/usr/include/c++/11/bits/unordered_set.h:378:23:   required from ‘std::pair<typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator, bool> std::unordered_set<_Value, _Hash, _Pred, _Alloc>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >; typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator = std::__detail::_Insert_base<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int> >, std::__detail::_Identity, std::equal_to<std::pair<int, int> >, std::hash<std::pair<int, int> >, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >::iterator]’
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:51:   required from here
/usr/include/c++/11/bits/hashtable_policy.h:1217:23: error: static assertion failed: hash function must be invocable with an argument of key type
 1217 |         static_assert(__is_invocable<const _Hash&, const _Key&>{},
      |                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/11/bits/hashtable_policy.h:1217:23: note: ‘std::__is_invocable<const std::hash<std::pair<int, int> >&, const std::pair<int, int>&>{}’ evaluates to false
/usr/include/c++/11/bits/hashtable_policy.h:1219:25: error: no match for call to ‘(const std::hash<std::pair<int, int> >) (const std::pair<int, int>&)’
 1219 |         return _M_hash()(__k);
      |                ~~~~~~~~~^~~~~
```


What's the problem?
==================================================
