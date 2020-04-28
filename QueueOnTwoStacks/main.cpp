#include <iostream>
#include "Stack.h"
#include "Queue.h"

int main() {
//    Stack s1;
//    std::cout << s1.Size() << '\n';
//    s1.Push(35);
//    s1.Push(2);
//    std::cout << s1.Size() << "  " << s1.Top() << '\n';
//    Stack s2 = s1;
//    std::cout << s2.Size() << "  " << s2.Top() << '\n';
//    s1.Clear();
//    std::cout << s1.Size() << '\n';
    Queue q1;
    std::cout << q1.Size() << '\n';
    q1.Push(4);
    q1.Push(35);
    std::cout << q1.Size() << "  " << q1.Front() << "\n";
    Queue q2 = q1;
    std::cout << q2.Size() << "  " << q2.Front() << "\n";
    return 0;
}
