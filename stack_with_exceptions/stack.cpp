#include <iostream>
#include <cstdint>
#include "stack.h"
#include "exc.h"
#include <stdexcept>

Stack::Stack() : head_(nullptr), size_(0) {
}

Stack::~Stack() {
    Clear();
}

void Stack::Push(T value) {
    try {
        Node* newPrev = new Node(head_, value);
        head_ = newPrev;
        ++size_;
    } catch (std::bad_alloc& ex) {
        throw BadAlloc(std::string("No memory"));
    }
}

void Stack::Pop() {
    if (Empty()) {
        throw RangeError(std::string("No more elements"));
    }
    Node* newTop = head_->prev_;
    delete head_;
    head_ = newTop;
    --size_;
}

T Stack::Top() const {
    if (!Empty()) {
        return head_->value_;
    } else {
        throw RangeError(std::string("No elements"));
    }
}

T& Stack::Top() {
    if (!Empty()) {
        return head_->value_;
    } else {
        throw RangeError(std::string("No elements"));
    }
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
    this->size_ = other.size_;

    T* values = new T[size_];
    Node* element = other.head_;
    for (size_t i = 0; i < other.size_; ++i) {
        values[i] = element->value_;
        element = element->prev_;
    }

    head_ = nullptr;
    try {
        for (ssize_t i = size_ - 1; i >= 0; --i) {
            Node* key = new Node(head_, values[i]);
            head_ = key;
        }
    } catch (std::bad_alloc& ex) {
        delete[] values;
        throw BadAlloc(std::string("No memory"));
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
