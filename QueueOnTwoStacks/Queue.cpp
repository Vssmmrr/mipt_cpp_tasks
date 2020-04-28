#include <cstddef>
#include "Stack.h"
#include "Queue.h"
typedef int T;

void Queue::Push(T value) {
    push_stack_.Push(value);
}

void Queue::Pop() {
    if (Empty()) {
        return;
    }

    if (pop_stack_.Empty()) {
        TransferValues();
    }

    pop_stack_.Pop();
}

void Queue::TransferValues() const {
    while (!push_stack_.Empty()) {
        pop_stack_.Push(push_stack_.Top());
        push_stack_.Pop();
    }
}

T& Queue::Front() {
        if (pop_stack_.Empty()) {
            TransferValues();
        }

        return pop_stack_.Top();
}

T Queue::Front() const {
        if (pop_stack_.Empty()) {
            TransferValues();
        }
        return pop_stack_.Top();
}

void Queue::Clear() {
    push_stack_.Clear();
    pop_stack_.Clear();
}

bool Queue::Empty() const {
    return push_stack_.Empty() && pop_stack_.Empty();
}

size_t Queue::Size() const {
    return (push_stack_.Size() + pop_stack_.Size());
}

/*
void Queue::Copy(const Queue& other) {
    push_stack_ = other.push_stack_;
    pop_stack_ = other.pop_stack_;
}

Queue::Queue(const Queue& other) {
    Copy(other);
}

Queue& Queue::operator=(const Queue& other) {
    if (this == &other) {
        return *this;
    }

    Copy(other);
    return *this;
}
 */