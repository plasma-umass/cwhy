#include <numeric>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }
    std::erase_if(v, [](int i) { return i % 3 == 0 || i % 5 == 0; });
    std::cout << std::accumulate(v.begin(), v.end(), 0) << std::endl;
}
