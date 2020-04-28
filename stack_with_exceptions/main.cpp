#include "stack.h"
#include "exc.h"
#include <iostream>

int main() {
    Stack s1;
    std::cout << s1.Size() << '\n';
    std::cout << s1.Empty() << '\n';

    try {
        s1.Push(5);
        s1.Push(6);
    } catch (BadAlloc& ex) {
        std::cout << ex.What() << '\n';
    }

    try {
        std::cout << s1.Top() << '\n';
        s1.Pop();
        std::cout << s1.Top() << '\n';
    } catch (RangeError& ex) {
        std::cout << ex.What() << '\n';
    }

    s1.Clear();
    std::cout << s1.Empty() << '\n';

    try {
        Stack s2 = s1;
        std::cout << s2.Top() << '\n';
        std::cout << s1.Top() << '\n';
        std::cout << s2.Size() << '\n';
    } catch (RangeError& exc) {
        std::cout << exc.What() << '\n';
    } catch (BadAlloc& ex) {
        std::cout << ex.What() << '\n';
    }

    return 0;
}
