/*
==================================================
CWhy
==================================================
Your regular expression `"^[a--z]++([0-9]++)$"` is invalid.

Your intention here appears to be to match any string which starts
with one or more lowercase letters and ends with one or more digits.
However, your character class range `a--z` seems to be incorrectly
typed. In regular expressions, a single `-` is used to represent a
range, so this should be `a-z` instead:

So the corrected regular expression should look like this:
```cpp
"^[a-z]++([0-9]++)$"
```

The corrected code should then be:

```cpp
1 #include <ctre.hpp>
2 #include <optional>
3 
4 std::optional<std::string_view> extract_number(std::string_view s) noexcept {
5     using namespace ctre::literals;
6     if (auto m = ctre::match<"^[a-z]++([0-9]++)$">(s)) {
7         return m.get<1>().to_view();
8     } else {
9         return std::nullopt;
10     }
```

When your original code tried to compile, the compiler tried to
compile an instantiation of `problem_at_position<4>`, in other words
it was trying to tell you there was a problem at the 4th character in
your regular expression, which is where your `--` is located. Once you
fix your regular expression to `"^[a-z]++([0-9]++)$"` the code should
compile without issues.

This kind of messages can help you debug your regular expressions in
the future by helping you locate problematic parts of your regular
expression. So when you see `problem_at_position<n>`, you can look at
the nth character in your regular expression for potential issues.

(Total cost: approximately $0.06 USD.)
==================================================
*/

// from https://gcc.godbolt.org/z/a5Kavxjab

#include <ctre.hpp>
#include <optional>
std::optional<std::string_view> extract_number(std::string_view s) noexcept {
    using namespace ctre::literals;
    if (auto m = ctre::match<"^[a--z]++([0-9]++)$">(s)) {
        return m.get<1>().to_view();
    } else {
        return std::nullopt;
    }
}
