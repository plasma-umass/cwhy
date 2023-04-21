/*
The problem is that you are trying to push a pointer variable (`int*
pointer`) into a vector of integers (`std::vector<int>`), which is
not allowed. The error message suggests that you need to dereference
the pointer by using `*pointer` to get the integer value it points to
and then push that value into the vector. The corrected code would
look like this:

```
int main() {
  std::vector<int> v;
  int value = 10;
  int* pointer = &value;
  v.push_back(*pointer);
}
```
*/
#include <vector>

int main() {
    std::vector<int> v;
    int value = 10;
    int* pointer = &value;
    v.push_back(pointer);
}
