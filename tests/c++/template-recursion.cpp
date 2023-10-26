#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <span>
#include <vector>

template <std::uint32_t M, std::uint32_t N>
struct Matrix {
    Matrix(std::span<std::uint64_t> underlying) : underlying(underlying) {
        assert(underlying.size() == M * N);
    }

    std::uint64_t lowestRowSum() const {
        const auto firstRowSum
            = std::accumulate(underlying.begin(), std::next(underlying.begin(), N), std::uint64_t(0));
        const auto subMatrix = Matrix<M - 1, N>(underlying.subspan(N));
        return std::min(firstRowSum, subMatrix.lowestRowSum());
    }

  private:
    // The Matrix object does not own the underlying memory, and is just used to perform operations.
    std::span<std::uint64_t> underlying;
};

#define M 100
#define N 10

int main() {
    std::vector<std::uint64_t> v(M * N);
    for (int i = 0; i < M * N; ++i) {
        v[i] = i;
    }
    const auto lowestRowSum = Matrix<M, N>(v).lowestRowSum();
}
