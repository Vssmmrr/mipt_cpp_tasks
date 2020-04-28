#ifndef QUEUEONTWOSTACKS_QUEUE_H
#define QUEUEONTWOSTACKS_QUEUE_H

#include <cstddef>
#include "Stack.h"
typedef int T;

class Queue {
private:
    //size_t size_;
    mutable Stack push_stack_;
    mutable Stack pop_stack_;

    void TransferValues() const;
    //void Copy(const Queue& other);
public:
    void Push(T value);
    void Pop();
    T& Front();
    T Front() const;
    void Clear();
    bool Empty() const;
    size_t Size() const;

    Queue() = default;
    Queue(const Queue& other) = default;
    Queue& operator=(const Queue& other) = default;
    ~Queue() = default;
};

#endif //QUEUEONTWOSTACKS_QUEUE_H