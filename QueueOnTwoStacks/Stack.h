#ifndef QUEUEONTWOSTACKS_STACK_H
#define QUEUEONTWOSTACKS_STACK_H

#include <cstddef>
typedef int T;

struct Node {
    Node* prev_;
    T value_;

    Node(Node* prev, T value) : prev_(prev), value_(value){
    }
};

class Stack {
private:
    Node* head_;
    size_t size_;

    void Copy(const Stack& other);

public:
    void Push(T value);
    void Pop();
    T Top() const;
    T& Top();
    void Clear();
    bool Empty() const;
    size_t Size() const;

    Stack();
    Stack(const Stack& other);

    Stack& operator=(const Stack& other);

    ~Stack();
};

#endif //QUEUEONTWOSTACKS_STACK_H
