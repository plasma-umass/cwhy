template <typename T>
T f(T t0, T t1) {
    return {};
}

template <typename T>
auto g(T t) -> decltype(f(t)) {
    return f(t);
}

int main() {
    g(17);
}
