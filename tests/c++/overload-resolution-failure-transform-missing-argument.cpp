#include <algorithm>
#include <vector>

int main() {
    std::vector<int> v;
    std::transform(v.begin(), v.end(), [](int i) { return i * i; });
}
