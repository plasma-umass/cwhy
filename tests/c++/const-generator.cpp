#include <iostream>
#include <random>

int main() {
    const std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(1, 100);
    for (int i = 0; i < 10; ++i) {
        int random = distribution(generator);
        std::cout << random << std::endl;
    }
}
