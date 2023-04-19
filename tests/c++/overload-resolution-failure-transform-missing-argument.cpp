/*
The problem is that you're not providing an output iterator as an
argument to `std::transform`. The function has the following
signature:

```
template<typename _InputIterator, typename _OutputIterator, typename _UnaryOperation>
    _OutputIterator
    transform(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _UnaryOperation __unary_op)
```

It requires an input iterator range (which you provided with
`v.begin()` and `v.end()`), an output iterator (which you haven't
provided), and a unary operation (your lambda function).

To fix the issue, you need to provide an output iterator as well. You
can use `std::back_inserter` to automatically insert the result of the
transformation into your vector:

```cpp
#include <algorithm>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v;
    std::vector<int> v_out;
    std::transform(v.begin(), v.end(), std::back_inserter(v_out), [](int i) { return i * i; });
    return 0;
}
```

In this example, the `std::back_inserter(v_out)` creates an output
iterator that appends new elements to the `v_out` vector.
*/
#include <algorithm>
#include <vector>

int main() {
    std::vector<int> v;
    std::transform(v.begin(), v.end(), [](int i) { return i * i; });
}
