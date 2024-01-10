===================== Prompt =====================
This is my code:

File `/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp`:
```
270 template <typename Range, typename RE, typename Modifier> constexpr auto operator|(Range && range, regular_expression<RE, iterat...
271 
272 template <typename Range, typename RE, typename Method, typename Modifier> constexpr auto operator|(Range && range, regular_expr...
273 	return re.multi_exec(std::forward<Range>(range));
274 }
275 
276 // error reporting of problematic position in a regex
277 template <size_t> struct problem_at_position; // do not define!
278 
279 template <> struct problem_at_position<~static_cast<size_t>(0)> {
280 	constexpr operator bool() const noexcept {
```

File `/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp`:
```
293 template <CTRE_REGEX_TEMPLATE_COPY_TYPE input> struct regex_builder {
294 	static constexpr auto _input = input;
295 	using result = typename ctll::parser<ctre::pcre, _input, ctre::pcre_actions>::template output<pcre_context<>>;
296 
297 	static constexpr auto n = result::is_correct ? ~static_cast<size_t>(0) : result::position;
298 
299 	static_assert(result::is_correct && problem_at_position<n>{}, "Regular Expression contains syntax error.");
300 
301 	using type = ctll::conditional<result::is_correct, decltype(ctll::front(typename result::output_type::stack_type())), ctll::lis...
302 };
303 
304 // case-sensitive
305 
306 template <CTRE_REGEX_INPUT_TYPE input, typename... Modifiers> static constexpr inline auto match = regular_expression<typename r...
307 
308 template <CTRE_REGEX_INPUT_TYPE input, typename... Modifiers> static constexpr inline auto search = regular_expression<typename ...
```

File `/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp`:
```
49 // from https://gcc.godbolt.org/z/a5Kavxjab
50 
51 #include <ctre.hpp>
52 #include <optional>
53 std::optional<std::string_view> extract_number(std::string_view s) noexcept {
54     using namespace ctre::literals;
55     if (auto m = ctre::match<"^[a--z]++([0-9]++)$">(s)) {
56         return m.get<1>().to_view();
57     } else {
58         return std::nullopt;
59     }
```

File `/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/return_type.hpp`:
```
308 	constexpr CTRE_FORCE_INLINE regex_results() noexcept { }
309 	constexpr CTRE_FORCE_INLINE regex_results(not_matched_tag_t) noexcept { }
310 
311 	// special constructor for deducting
312 	constexpr CTRE_FORCE_INLINE regex_results(Iterator, ctll::list<Captures...>) noexcept { }
313 
314 	template <size_t Id, typename = std::enable_if_t<decltype(_captures)::template exists<Id>()>> CTRE_FORCE_INLINE constexpr auto ...
315 		return _captures.template select<Id>();
316 	}
317 	template <typename Name, typename = std::enable_if_t<decltype(_captures)::template exists<Name>()>> CTRE_FORCE_INLINE constexpr...
318 		return _captures.template select<Name>();
319 	}
320 #if CTRE_CNTTP_COMPILER_CHECK
321 	template <ctll::fixed_string Name, typename = std::enable_if_t<decltype(_captures)::template exists<Name>()>> CTRE_FORCE_INLINE...
322 #else
323 	template <const auto & Name, typename = std::enable_if_t<decltype(_captures)::template exists<Name>()>> CTRE_FORCE_INLINE const...
324 #endif
```

File `/usr/include/c++/12/type_traits`:
```
2602   /// Alias template for decay
2603   template<typename _Tp>
2604     using decay_t = typename decay<_Tp>::type;
2605 
2606   /// Alias template for enable_if
2607   template<bool _Cond, typename _Tp = void>
2608     using enable_if_t = typename enable_if<_Cond, _Tp>::type;
2609 
2610   /// Alias template for conditional
2611   template<bool _Cond, typename _Iftrue, typename _Iffalse>
```

File `/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp`:
```
37 	else return {first_unit, 1};
38 }
39 
40 struct construct_from_pointer_t { };
41 
42 constexpr auto construct_from_pointer = construct_from_pointer_t{};
43 
44 template <size_t N> struct fixed_string {
45 	char32_t content[N] = {};
46 	size_t real_size{0};
47 	bool correct_flag{true};
48 
49 	template <typename T> constexpr fixed_string(construct_from_pointer_t, const T * input) noexcept {
50 		if constexpr (std::is_same_v<T, char>) {
51 			#ifdef CTRE_STRING_IS_UTF8
52 				size_t out{0};
```

File `/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp`:
```
142 				if ((i == (N-1)) && (input[i] == 0)) break;
143 				real_size++;
144 			}
145 		}
146 	}
147 
148 	template <typename T> constexpr fixed_string(const std::array<T, N> & in) noexcept: fixed_string{construct_from_pointer, in.dat...
149 	template <typename T> constexpr fixed_string(const T (&input)[N+1]) noexcept: fixed_string{construct_from_pointer, input} { }
150 
151 	constexpr fixed_string(const fixed_string & other) noexcept {
152 		for (size_t i{0}; i < N; ++i) {
153 			content[i] = other.content[i];
154 		}
```

File `/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp`:
```
211 	}
212 	constexpr operator std::basic_string_view<char32_t>() const noexcept {
213 		return std::basic_string_view<char32_t>{empty, 0};
214 	}
215 };
216 
217 template <typename CharT, size_t N> fixed_string(const CharT (&)[N]) -> fixed_string<N-1>;
218 template <typename CharT, size_t N> fixed_string(const std::array<CharT,N> &) -> fixed_string<N>;
219 
220 template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;
221 
222 }
```


This is my error:
```
In file included from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/literals.hpp:7,
                 from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre.hpp:4,
                 from /home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:51:
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp: In instantiation of ‘struct ctre::regex_builder<ctll::fixed_string<19>{char32_t [19]{94, 91, 97, 45, 45, 122, 93, 43, 43, 40, 91, 48, 45, 57, 93, 43, 43, 41, 36}, 19, true}>’:
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp:306:100:   required from ‘constexpr const auto ctre::match<ctll::fixed_string<19>{char32_t [19]{94, 91, 97, 45, 45, 122, 93, 43, 43, 40, 91, 48, 45, 57, 93, 43, 43, 41, 36}, 19, true}>’
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:55:24:   required from here
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp:299:42: error: invalid use of incomplete type ‘struct ctre::problem_at_position<4>’
  299 |         static_assert(result::is_correct && problem_at_position<n>{}, "Regular Expression contains syntax error.");
      |                               ~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp:277:26: note: declaration of ‘struct ctre::problem_at_position<4>’
  277 | template <size_t> struct problem_at_position; // do not define!
      |                          ^~~~~~~~~~~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp: In function ‘std::optional<std::basic_string_view<char> > extract_number(std::string_view)’:
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:56:24: error: no matching function for call to ‘ctre::regex_results<const char*>::get<1>()’
   56 |         return m.get<1>().to_view();
      |                ~~~~~~~~^~
In file included from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/evaluation.hpp:9,
                 from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/literals.hpp:6:
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/return_type.hpp:314:136: note: candidate: ‘template<long unsigned int Id, class> constexpr auto ctre::regex_results<Iterator, Captures>::get() const [with long unsigned int Id = Id; Iterator = const char*; Captures = {}]’
  314 |         template <size_t Id, typename = std::enable_if_t<decltype(_captures)::template exists<Id>()>> CTRE_FORCE_INLINE constexpr auto get() const noexcept {
      |                                                                                                                                        ^~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/return_type.hpp:314:136: note:   template argument deduction/substitution failed:
In file included from /usr/include/c++/12/bits/stl_pair.h:60,
                 from /usr/include/c++/12/utility:69,
                 from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:4,
                 from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/parser.hpp:4,
                 from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll.hpp:4,
                 from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/literals.hpp:4:
/usr/include/c++/12/type_traits: In substitution of ‘template<bool _Cond, class _Tp> using enable_if_t = typename std::enable_if::type [with bool _Cond = false; _Tp = void]’:

[...]

      |                ~~~~~~~~^~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:44:28: note: candidate: ‘template<long unsigned int N> fixed_string(ctll::fixed_string<N>)-> ctll::fixed_string<N>’
   44 | template <size_t N> struct fixed_string {
      |                            ^~~~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:44:28: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:56:24: note:   mismatched types ‘ctll::fixed_string<N>’ and ‘int’
   56 |         return m.get<1>().to_view();
      |                ~~~~~~~~^~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:220:21: note: candidate: ‘template<long unsigned int N> ctll::fixed_string(fixed_string<N>)-> fixed_string<N>’
  220 | template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;
      |                     ^~~~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:220:21: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:56:24: note:   mismatched types ‘ctll::fixed_string<N>’ and ‘int’
   56 |         return m.get<1>().to_view();
      |                ~~~~~~~~^~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:218:37: note: candidate: ‘template<class CharT, long unsigned int N> ctll::fixed_string(const std::array<_Tp, _Nm>&)-> fixed_string<N>’
  218 | template <typename CharT, size_t N> fixed_string(const std::array<CharT,N> &) -> fixed_string<N>;
      |                                     ^~~~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:218:37: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:56:24: note:   mismatched types ‘const std::array<_Tp, _Nm>’ and ‘int’
   56 |         return m.get<1>().to_view();
      |                ~~~~~~~~^~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:217:37: note: candidate: ‘template<class CharT, long unsigned int N> ctll::fixed_string(const CharT (&)[N])-> fixed_string<(N - 1)>’
  217 | template <typename CharT, size_t N> fixed_string(const CharT (&)[N]) -> fixed_string<N-1>;
      |                                     ^~~~~~~~~~~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/../ctll/fixed_string.hpp:217:37: note:   template argument deduction/substitution failed:
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:56:24: note:   mismatched types ‘const CharT [N]’ and ‘int’
   56 |         return m.get<1>().to_view();
      |                ~~~~~~~~^~
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
