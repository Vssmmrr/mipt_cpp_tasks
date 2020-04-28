#include "circular_buffer.h"
#include <iostream>
#include <utility>


int main() {
    CircularBuffer<int> a(5);
    CircularBuffer<int> b(5, 6);
    a.PushBack(4);
    b.PushFront(7);
    a.Swap(b);
    std::cout << a.Capacity() << '\n';
    std::cout << a.Size() << '\n';
    b.Clear();
    std::cout << b.Empty() << '\n';
    std::cout << a.Front() << '\n';
    std::cout << a[0] << '\n';
    std::cout << b.Back() << '\n';
    a.Reserve(20);
    std::cout << a.Capacity() << '\n';
    std::cout << a.Back() << '\n';
    return 0;
}
