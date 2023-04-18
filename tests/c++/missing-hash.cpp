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
