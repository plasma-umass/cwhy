/*
The problem is that you are trying to output the `Cat` object using
the `<<` operator, but you haven't defined an overload for that
operator to handle the `Cat` type. You can fix this by defining an
overloaded `operator<<` that takes an `ostream` reference and a `Cat`
reference, then outputs the desired information from the `Cat` object.
Here's how you might do that in the `Cat` class:

```cpp
friend std::ostream& operator<<(std::ostream& os, const Cat& cat) {
    return os << "Cat(" << cat.name << ", " << cat.age << ")";
}
```

Note that the function is declared using the `friend` keyword so that
it can access private members of the `Cat` class.
*/
#include <iostream>

struct Cat {
    Cat(std::string name, int age) : name(std::move(name)), age(age) {}

  private:
    const std::string name;
    const int age;
};

int main() {
    Cat cat("Scott", 13);
    std::cout << cat << std::endl;
}
