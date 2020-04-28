#include <iostream>
#include <cstddef>
#include "Stack.h"

Stack::Stack() : head_(nullptr), size_(0) {
}

Stack::~Stack() {
    Clear();
}

void Stack::Push(T value) {
    Node* newPrev = new Node(head_, value);
    head_ = newPrev;
    ++size_;
}

void Stack::Pop() {
    if (Empty()) {
        return;
    }
    Node* newTop = head_->prev_;
    delete head_;
    head_ = newTop;
    --size_;
}

T Stack::Top() const {
    return head_->value_;

}

T& Stack::Top() {
    return head_->value_;
}

bool Stack::Empty() const {
    return size_ == 0;
}

size_t Stack::Size() const {
    return size_;
}

void Stack::Clear() {
    while (!Empty()) {
        Pop();
    }
}

void Stack::Copy(const Stack& other) {
    this->Clear();
    //this->size_ = other.size_;

    T* values = new T[size_];
    Node* element = other.head_;
    for(size_t i = 0; i < other.size_; ++i) {
        values[i] = element->value_;
        element = element->prev_;
    }

    head_ = nullptr;
    for (ssize_t i = other.size_ - 1; i >= 0; --i) {
        Push(values[i]);
    }

    delete[] values;
}

Stack& Stack::operator=(const Stack& other) {
    if (this == &other) {
        return *this;
    }

    Copy(other);
    return *this;
}

Stack::Stack(const Stack& other) : Stack() {
    Copy(other);
}
