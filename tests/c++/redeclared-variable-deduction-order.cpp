template <typename T>
T f(T t) {
    return {};
}

extern decltype(f(0)) g;

double f(int i) {
    return {};
}

decltype(f(0)) g = {};
