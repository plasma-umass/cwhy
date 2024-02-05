# CWhy

by [Emery Berger](https://emeryberger.com),
[Nicolas van Kempen](https://nvankempen.com/),
and [Bryce Adelstein Lelbach](https://twitter.com/blelbach).

[![PyPI](https://img.shields.io/pypi/v/cwhy.svg)](https://pypi.org/project/cwhy/)
[![downloads](https://static.pepy.tech/badge/cwhy)](https://pepy.tech/project/cwhy)
[![downloads/month](https://static.pepy.tech/badge/cwhy/month)](https://pepy.tech/project/cwhy)

("See why")

Explains and suggests fixes for compiler error messages for a wide range of programming languages, including C, C++, C#,
Go, Java, LaTeX, PHP, Python, Ruby, Rust, Swift, and TypeScript.

## Installation

 >  **Note**
 >
 >  CWhy needs to be connected to an [OpenAI account](https://openai.com/api/) or an Amazon Web Services account.
 >  _Your account will need to have a
 >  positive balance for this to work_ ([check your OpenAI balance](https://platform.openai.com/usage)).
 >  [Get an OpenAI key here](https://platform.openai.com/api-keys).
 > 
 >  CWhy currently defaults to GPT-4, and falls back to GPT-3.5-turbo if a request error occurs. For the newest and best
 >  model (GPT-4) to work, you need to have purchased  at least $1 in credits (if your API account was created before
 >  August 13, 2023) or $0.50 (if you have a newer API account).
 > 
 >  Once you have an API key, set it as an environment variable called `OPENAI_API_KEY`.
 > 
 >  ```bash
 >  # On Linux/MacOS:
 >  export OPENAI_API_KEY=<your-api-key>
 >  
 >  # On Windows:
 >  $env:OPENAI_API_KEY=<your-api-key>
 >  ```
 >
 >  **New**: CWhy now has alpha support for Amazon Bedrock, using the Claude model.
 >  To use Bedrock, you need to set three environment variables.
 >
 >  ```bash
 >  # On Linux/MacOS:
 >  export AWS_ACCESS_KEY_ID=<your-access-key>
>   export AWS_SECRET_ACCESS_KEY=<your-secret-key>
>   export AWS_REGION_NAME=us-west-2
>   ```
>
>   CWhy will automatically select which AI service to use (OpenAI or AWS Bedrock)
>   when it detects that the appropriate environment variables have been set.
 
```
python3 -m pip install cwhy
```

## Usage 

### Linux/MacOS

The wrapper mode is now default and mandatory, with a slightly modified interface.
CWhy can either be used standalone by passing the full command after the triple dashes `---`, or as part of a build tool
by creating a short executable script wrapping the compiler command.

```bash
# Invoking the compiler directly.
% cwhy --- g++ mycode.cpp

# Using CWhy with Java and an increased timeout.
% cwhy --timeout 180 --- javac MyCode.java

# Invoking with GNU Make, using GPT-3.5.
% CXX=`cwhy --llm=gpt-3.5-turbo --wrapper --- c++` make

# Invoking with CMake, using GPT-4 and clang++.
% cmake -DCMAKE_CXX_COMPILER=`cwhy --llm=gpt-4 --wrapper --- clang++` ...
```

When running a configuration tool such as CMake or Autoconf, this may greatly increase configuration time, as these
tools will occasionally invoke the compiler to check for features, which will fail and invoke CWhy unnecessarily if not
available on the machine. To circumvent this, `CWHY_DISABLE` can be set in the environment to disable CWhy at
configuration time.

```bash
% CWHY_DISABLE='ON' cmake -DCMAKE_CXX_COMPILER=`cwhy --wrapper --- c++` ...
```

### Windows

Windows support has been tested using Powershell. On the command line, using Ninja is required as MSBuild / `.vcxproj`
will override any option set.

```bash
% $env:CWHY_DISABLE='ON'
% cmake -G Ninja -DCMAKE_CXX_COMPILER="$(python -m cwhy --wrapper --- cl)"  ...
% $env:CWHY_DISABLE=''
```

### Continuous Integration

CI using GitHub actions is straightforward on both Linux and MacOs. On Windows, Ninja is not installed by default on the
image, and `cl` is not bound to the compiler. We recommend using `choco install ninja` and
[ilammy/msvc-dev-cmd](https://github.com/ilammy/msvc-dev-cmd) to work around these two issues.

An example action YAML file covering all three platforms
[can be found here](https://github.com/nicovank/litterer/blob/master/.github/workflows/ci.yml).

**Important**: Set the `CWHY_DISABLE` environment variable at configure-time to save money and cycles.

### Options

These options can be displayed with `cwhy --help`.

 -  `--llm`: pick a specific OpenAI LLM. CWhy has been tested with `gpt-3.5-turbo` and `gpt-4`.
 -  `--timeout`: pick a different timeout than the default for API calls.
 -  `--show-prompt` (debug): print prompts before calling the API.

## Examples

### C++

This highlighted example is [missing-hash.cpp](tests/c++/missing-hash.cpp), which is one of the first cases we
experimented with.

<details>
<summary>
Expand to see the original (pretty obscure) error message:
</summary>

```
% clang++ --std=c++20 -c missing-hash.cpp
missing-hash.cpp:13:45: error: call to implicitly-deleted default constructor of 'std::unordered_set<std::pair<int, int>>'
    std::unordered_set<std::pair<int, int>> visited;
                                            ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/unordered_set.h:135:7: note: explicitly defaulted function was implicitly deleted here
      unordered_set() = default;
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/unordered_set.h:100:18: note: default constructor of 'unordered_set<std::pair<int, int>>' is implicitly deleted because field '_M_h' has a deleted default constructor
      _Hashtable _M_h;
                 ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable.h:451:7: note: explicitly defaulted function was implicitly deleted here
      _Hashtable() = default;
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable.h:174:7: note: default constructor of '_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>' is implicitly deleted because base class '__detail::_Hashtable_base<pair<int, int>, pair<int, int>, _Identity, equal_to<pair<int, int>>, hash<pair<int, int>>, _Mod_range_hashing, _Default_ranged_hash, _Hashtable_traits<true, true, true>>' has a deleted default constructor
    : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1791:5: note: explicitly defaulted function was implicitly deleted here
    _Hashtable_base() = default;
    ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1726:5: note: default constructor of '_Hashtable_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Hashtable_traits<true, true, true>>' is implicitly deleted because base class '_Hash_code_base<pair<int, int>, pair<int, int>, _Identity, hash<pair<int, int>>, _Mod_range_hashing, _Default_ranged_hash, _Hashtable_traits<true, true, true>::__hash_cached::value>' has a deleted default constructor
  : public _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash,
    ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1368:7: note: explicitly defaulted function was implicitly deleted here
      _Hash_code_base() = default;
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1344:7: note: default constructor of '_Hash_code_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, true>' is implicitly deleted because base class '_Hashtable_ebo_helper<1, hash<pair<int, int>>>' has a deleted default constructor
      private _Hashtable_ebo_helper<1, _H1>,
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1112:7: note: explicitly defaulted function was implicitly deleted here
      _Hashtable_ebo_helper() = default;
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1110:7: note: default constructor of '_Hashtable_ebo_helper<1, std::hash<std::pair<int, int>>, true>' is implicitly deleted because base class 'std::hash<std::pair<int, int>>' has a deleted default constructor
    : private _Tp
      ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/functional_hash.h:101:19: note: default constructor of 'hash<std::pair<int, int>>' is implicitly deleted because base class '__hash_enum<pair<int, int>>' has no default constructor
    struct hash : __hash_enum<_Tp>
                  ^
In file included from missing-hash.cpp:1:
In file included from /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/functional:61:
In file included from /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/unordered_map:46:
In file included from /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable.h:35:
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1377:2: error: static assertion failed due to requirement 'std::__is_invocable<const std::hash<std::pair<int, int>> &, const std::pair<int, int> &>{}': hash function must be invocable with an argument of key type
        static_assert(__is_invocable<const _H1&, const _Key&>{},
        ^             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable.h:1675:29: note: in instantiation of member function 'std::__detail::_Hash_code_base<std::pair<int, int>, std::pair<int, int>, std::__detail::_Identity, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, true>::_M_hash_code' requested here
        __hash_code __code = this->_M_hash_code(__k);
                                   ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable.h:788:11: note: in instantiation of function template specialization 'std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>::_M_emplace<const std::pair<int, int> &>' requested here
        { return _M_emplace(__unique_keys(), std::forward<_Args>(__args)...); }
                 ^
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/unordered_set.h:377:16: note: in instantiation of function template specialization 'std::_Hashtable<std::pair<int, int>, std::pair<int, int>, std::allocator<std::pair<int, int>>, std::__detail::_Identity, std::equal_to<std::pair<int, int>>, std::hash<std::pair<int, int>>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true>>::emplace<const std::pair<int, int> &>' requested here
        { return _M_h.emplace(std::forward<_Args>(__args)...); }
                      ^
missing-hash.cpp:20:44: note: in instantiation of function template specialization 'std::unordered_set<std::pair<int, int>>::emplace<const std::pair<int, int> &>' requested here
        const auto [_, inserted] = visited.emplace(n->position);
                                           ^
In file included from missing-hash.cpp:1:
In file included from /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/functional:61:
In file included from /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/unordered_map:46:
In file included from /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable.h:35:
/usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/hashtable_policy.h:1379:9: error: type 'const std::hash<std::pair<int, int>>' does not provide a call operator
        return _M_h1()(__k);
               ^~~~~~~
3 errors generated.
```
</details>

And here's the English-language explanation from `cwhy`:

````
% cwhy --- clang++ --std=c++20 -c missing-hash.cpp
The problem is that you are attempting to use `std::unordered_set`
with `std::pair<int, int>` as the key type. However, the standard
library does not provide a hash function specialization for
`std::pair` out of the box, so the default constructor of the
unordered set is deleted.

To resolve this, you'll need to provide a custom hash function for
`std::pair<int, int>`. Here's an example of how you can define one:

```cpp
struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        std::hash<T1> hash1;
        std::hash<T2> hash2;
        return hash1(pair.first) ^ (hash2(pair.second) << 1);
    }
};
```

Then, when instantiating the `std::unordered_set`, you can specify the
custom hash function:

```cpp
std::unordered_set<std::pair<int, int>, PairHash> visited;
```

With this change, the code should now compile and work as expected.
````


### Rust

```
% cwhy --- cargo build
There are three issues:  1. There are two unused variables `x` in the
code.  2. The variable `x` is used after it has already been moved in
the call to `f(x)`, which takes ownership of `x`. 3. The function
`f(x)` takes ownership of `x`, which may not be necessary and could be
changed to borrow the value instead.
```
