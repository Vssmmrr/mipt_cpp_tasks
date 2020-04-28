#ifndef STACK_H
#define STACK_H

#include <cstddef>
typedef int T;

struct Node {
    Node* prev_;
    T value_;

    Node(Node* prev, T value) : prev_(prev), value_(value) {
    }

};

class Stack {
private:
    Node* head_;
    size_t size_;

public:
    void Push(T value);
    void Pop();
    T Top() const ;
    T& Top();
    void Clear();
    bool Empty() const;
    size_t Size() const;
    void Copy(const Stack& other);

    Stack();
    Stack(const Stack& other);

    Stack& operator=(const Stack& other);

    ~Stack();

};

#endif //STACK_H
