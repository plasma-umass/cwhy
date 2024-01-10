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


This is my error:
```
In file included from /home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:51:
In file included from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre.hpp:4:
In file included from /home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/literals.hpp:7:
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp:299:38: error: implicit instantiation of undefined template 'ctre::problem_at_position<4>'
  299 |         static_assert(result::is_correct && problem_at_position<n>{}, "Regular Expression contains syntax error.");
      |                                             ^
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp:306:128: note: in instantiation of template class 'ctre::regex_builder<ctll::fixed_string<20UL - 1>{{94, 91, 97, 45, 45, 122, 93, 43, 43, 40, ...}, 19, true}>' requested here
  306 | template <CTRE_REGEX_INPUT_TYPE input, typename... Modifiers> static constexpr inline auto match = regular_expression<typename regex_builder<input>::type, match_method, ctll::list<singleline, Modifiers...>>();
      |                                                                                                                                ^
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:55:24: note: in instantiation of variable template specialization 'ctre::match' requested here
   55 |     if (auto m = ctre::match<"^[a--z]++([0-9]++)$">(s)) {
      |                        ^
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/wrapper.hpp:277:26: note: template is declared here
  277 | template <size_t> struct problem_at_position; // do not define!

[...]

      |                          ^
/home/runner/work/cwhy/cwhy/tests/c++/ctre-test.cpp:56:18: error: no matching member function for call to 'get'
   56 |         return m.get<1>().to_view();
      |                ~~^~~~~~
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/return_type.hpp:314:129: note: candidate template ignored: requirement 'captures<ctre::captured_content<0, void>::storage<const char *>>::exists()' was not satisfied [with Id = 1]
  314 |         template <size_t Id, typename = std::enable_if_t<decltype(_captures)::template exists<Id>()>> CTRE_FORCE_INLINE constexpr auto get() const noexcept {
      |                                                                                                                                        ^
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/return_type.hpp:317:135: note: candidate template ignored: invalid explicitly-specified argument for template parameter 'Name'
  317 |         template <typename Name, typename = std::enable_if_t<decltype(_captures)::template exists<Name>()>> CTRE_FORCE_INLINE constexpr auto get() const noexcept {
      |                                                                                                                                              ^
/home/runner/work/cwhy/cwhy/tests/_deps/c++/ctre-test.cpp/install/include/ctre/return_type.hpp:321:145: note: candidate template ignored: invalid explicitly-specified argument for template parameter 'Name'
  321 |         template <ctll::fixed_string Name, typename = std::enable_if_t<decltype(_captures)::template exists<Name>()>> CTRE_FORCE_INLINE constexpr auto get() const noexcept {
      |                                                                                                                                                        ^
2 errors generated.
```


What's the problem? If you can, suggest code to fix the issue.
==================================================
