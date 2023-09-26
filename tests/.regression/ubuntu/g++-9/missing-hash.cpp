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

File `/usr/include/c++/9/bits/unordered_set.h`:
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

File `/usr/include/c++/9/bits/unordered_set.h`:
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

File `/usr/include/c++/9/bits/unordered_set.h`:
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

File `/usr/include/c++/9/bits/hashtable.h`:
```
432       _Hashtable(_Hashtable&&, __node_alloc_type&&,
433 		 false_type /* alloc always equal */);
434 
435 
436     public:
437       // Constructor, destructor, assignment, swap
438       _Hashtable() = default;
439       _Hashtable(size_type __bucket_hint,
440 		 const _H1&, const _H2&, const _Hash&,
441 		 const _Equal&, const _ExtractKey&,
```

File `/usr/include/c++/9/bits/hashtable.h`:
```
776     public:
777       // Emplace
778       template<typename... _Args>
779 	__ireturn_type
780 	emplace(_Args&&... __args)
781 	{ return _M_emplace(__unique_keys(), std::forward<_Args>(__args)...); }
782 
783       template<typename... _Args>
784 	iterator
```

File `/usr/include/c++/9/bits/hashtable.h`:
```
1383     _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
1384 	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
1385     ~_Hashtable() noexcept
1386     {
1387       clear();
1388       _M_deallocate_buckets();
1389     }
1390 
1391   template<typename _Key, typename _Value,
1392 	   typename _Alloc, typename _ExtractKey, typename _Equal,
```

File `/usr/include/c++/9/bits/hashtable.h`:
```
1672 	// First build the node to get access to the hash code
1673 	__node_type* __node = this->_M_allocate_node(std::forward<_Args>(__args)...);
1674 	const key_type& __k = this->_M_extract()(__node->_M_v());
1675 	__hash_code __code;
1676 	__try
1677 	  {
1678 	    __code = this->_M_hash_code(__k);
1679 	  }
1680 	__catch(...)
1681 	  {
```

File `/usr/include/c++/9/bits/hashtable_policy.h`:
```
1086    */
1087   template<int _Nm, typename _Tp,
1088 	   bool __use_ebo = !__is_final(_Tp) && __is_empty(_Tp)>
1089     struct _Hashtable_ebo_helper;
1090 
1091   /// Specialization using EBO.
1092   template<int _Nm, typename _Tp>
1093     struct _Hashtable_ebo_helper<_Nm, _Tp, true>
1094     : private _Tp
1095     {
1096       _Hashtable_ebo_helper() = default;
1097 
1098       template<typename _OtherTp>
1099 	_Hashtable_ebo_helper(_OtherTp&& __tp)
```

File `/usr/include/c++/9/bits/hashtable_policy.h`:
```
1341   /// Specialization: hash function and range-hashing function,
1342   /// caching hash codes.  H is provided but ignored.  Provides
1343   /// typedef and accessor required by C++ 11.
1344   template<typename _Key, typename _Value, typename _ExtractKey,
1345 	   typename _H1, typename _H2>
1346     struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2,
1347 			   _Default_ranged_hash, true>
1348     : private _Hashtable_ebo_helper<0, _ExtractKey>,
1349       private _Hashtable_ebo_helper<1, _H1>,
```

File `/usr/include/c++/9/bits/hashtable_policy.h`:
```
1368     protected:
1369       typedef std::size_t 				__hash_code;
1370       typedef _Hash_node<_Value, true>			__node_type;
1371 
1372       // We need the default constructor for _Hashtable default constructor.
1373       _Hash_code_base() = default;
1374       _Hash_code_base(const _ExtractKey& __ex,
1375 		      const _H1& __h1, const _H2& __h2,
1376 		      const _Default_ranged_hash&)
1377       : __ebo_extract_key(__ex), __ebo_h1(__h1), __ebo_h2(__h2) { }
1378 
1379       __hash_code
1380       _M_hash_code(const _Key& __k) const
1381       {
1382 	static_assert(__is_invocable<const _H1&, const _Key&>{},
1383 	    "hash function must be invocable with an argument of key type");
1384 	return _M_h1()(__k);
1385       }
1386 
1387       std::size_t
```

File `/usr/include/c++/9/bits/hashtable_policy.h`:
```
1764    *    - __detail::_Hash_code_base
1765    *    - __detail::_Hashtable_ebo_helper
1766    */
1767   template<typename _Key, typename _Value,
1768 	   typename _ExtractKey, typename _Equal,
1769 	   typename _H1, typename _H2, typename _Hash, typename _Traits>
1770   struct _Hashtable_base
1771   : public _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash,
1772 			   _Traits::__hash_cached::value>,
1773     private _Hashtable_ebo_helper<0, _Equal>
```

File `/usr/include/c++/9/bits/hashtable_policy.h`:
```
1816   private:
1817     using _EqualEBO = _Hashtable_ebo_helper<0, _Equal>;
1818     using _EqualHelper =  _Equal_helper<_Key, _Value, _ExtractKey, _Equal,
1819 					__hash_code, __hash_cached::value>;
1820 
1821   protected:
1822     _Hashtable_base() = default;
1823     _Hashtable_base(const _ExtractKey& __ex, const _H1& __h1, const _H2& __h2,
1824 		    const _Hash& __hash, const _Equal& __eq)
1825     : __hash_code_base(__ex, __h1, __h2, __hash), _EqualEBO(__eq)
```


This is my error:
```
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp: In function ‘void bfs(Node*, std::function<void(Node*)>)’:
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:43:45: error: use of deleted function ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’
   43 |     std::unordered_set<std::pair<int, int>> visited;
      |                                             ^~~~~~~
In file included from /usr/include/c++/9/unordered_set:47,
                 from /home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:33:
/usr/include/c++/9/bits/unordered_set.h:135:7: note: ‘std::unordered_set<_Value, _Hash, _Pred, _Alloc>::unordered_set() [with _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >]’ is implicitly deleted because the default definition would be ill-formed:
  135 |       unordered_set() = default;
      |       ^~~~~~~~~~~~~
/usr/include/c++/9/bits/unordered_set.h:135:7: error: use of deleted function ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::_Hashtable() [with _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _H1 = std::hash<std::pair<int, int> >; _H2 = std::__detail::_Mod_range_hashing; _Hash = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>]’
In file included from /usr/include/c++/9/unordered_set:46,

[...]

/usr/include/c++/9/bits/hashtable.h:1678:13:   required from ‘std::pair<typename std::__detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal, _H1, _H2, _Hash, _Traits>::iterator, bool> std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::_M_emplace(std::true_type, _Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _H1 = std::hash<std::pair<int, int> >; _H2 = std::__detail::_Mod_range_hashing; _Hash = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; typename std::__detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal, _H1, _H2, _Hash, _Traits>::iterator = std::__detail::_Node_iterator<std::pair<int, int>, true, true>; std::true_type = std::integral_constant<bool, true>]’
/usr/include/c++/9/bits/hashtable.h:781:69:   required from ‘std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::__ireturn_type std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Key = std::pair<int, int>; _Value = std::pair<int, int>; _Alloc = std::allocator<std::pair<int, int> >; _ExtractKey = std::__detail::_Identity; _Equal = std::equal_to<std::pair<int, int> >; _H1 = std::hash<std::pair<int, int> >; _H2 = std::__detail::_Mod_range_hashing; _Hash = std::__detail::_Default_ranged_hash; _RehashPolicy = std::__detail::_Prime_rehash_policy; _Traits = std::__detail::_Hashtable_traits<true, true, true>; std::_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>::__ireturn_type = std::pair<std::__detail::_Node_iterator<std::pair<int, int>, true, true>, bool>]’
/usr/include/c++/9/bits/unordered_set.h:378:54:   required from ‘std::pair<typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator, bool> std::unordered_set<_Value, _Hash, _Pred, _Alloc>::emplace(_Args&& ...) [with _Args = {const std::pair<int, int>&}; _Value = std::pair<int, int>; _Hash = std::hash<std::pair<int, int> >; _Pred = std::equal_to<std::pair<int, int> >; _Alloc = std::allocator<std::pair<int, int> >; typename std::_Hashtable<_Value, _Value, _Alloc, std::__detail::_Identity, _Pred, _Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<std::__not_<std::__and_<std::__is_fast_hash<_Hash>, std::__is_nothrow_invocable<const _Hash&, const _Tp&> > >::value, true, true> >::iterator = std::__detail::_Node_iterator<std::pair<int, int>, true, true>]’
/home/runner/work/cwhy/cwhy/tests/c++/missing-hash.cpp:50:63:   required from here
/usr/include/c++/9/bits/hashtable_policy.h:1382:16: error: static assertion failed: hash function must be invocable with an argument of key type
 1382 |  static_assert(__is_invocable<const _H1&, const _Key&>{},
      |                ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/9/bits/hashtable_policy.h:1384:16: error: no match for call to ‘(const std::hash<std::pair<int, int> >) (const std::pair<int, int>&)’
 1384 |  return _M_h1()(__k);
      |         ~~~~~~~^~~~~
```


What's the problem?
==================================================
