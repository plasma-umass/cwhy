/*
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
*/
#include <functional>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

struct Node {
    const std::pair<int, int> position;
    std::vector<Node*> neighbors;
};

void bfs(Node* start, std::function<void(Node*)> f) {
    std::unordered_set<std::pair<int, int>> visited;
    std::queue<Node*> queue;
    queue.push(start);

    while (!queue.empty()) {
        auto* n = queue.front();
        queue.pop();
        const auto [_, inserted] = visited.emplace(n->position);
        if (inserted) {
            f(n);
            for (auto* neighbor : n->neighbors) {
                queue.push(neighbor);
            }
        }
    }
}
