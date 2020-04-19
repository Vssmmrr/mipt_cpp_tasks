#include <iostream>
#include "vector.h"


int main() {
    Vector<int> a(5);
    Vector<int> b(5, 7);
    std::cout << a.Capacity() << '\n';
    std::cout << b.Front() << '\n';
    Vector<int> c;
    std::cout << c.Data() << '\n';
    std::cout << a.Data() << '\n';
    return 0;
}
