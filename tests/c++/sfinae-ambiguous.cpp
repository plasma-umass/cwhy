void f(void*) {}
void f(char*) {}

template <typename T>
auto g(T t) -> decltype(f(t)) {
    return f(t);
}

int main() {
    g(nullptr);
}
