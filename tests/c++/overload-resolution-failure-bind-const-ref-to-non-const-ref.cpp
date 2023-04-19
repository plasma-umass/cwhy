void f(char*) {}

void f(float&) {}

int main() {
    float const i = 3.14;
    f(i);
}
