#ifndef PAGE_H
#define PAGE_H

#include <iostream>
#include <cstdint>

template <class T, size_t Capacity>
class Page {
private:
    T buffer_[Capacity];
    size_t size_ = 0;
    mutable int head_ = 0;
    mutable bool is_front_ = true;

public:
    Page() : size_(0), head_(0), is_front_(true) {
    }

    Page(const Page& other) = default;

    Page& operator=(const Page& other) = default;

    ~Page() = default;

    T& operator[](size_t idx) {
        return buffer_[head_ + idx];
    }

    T operator[](size_t idx) const {
        return buffer_[head_ + idx];
    }

    T& Front() {
        return buffer_[head_];
    }

    T& Back() {
        return buffer_[head_ + size_ - 1];
    }

    T Front() const {
        return buffer_[head_];
    }

    T Back() const {
        return buffer_[head_ + size_ - 1];
    }

    bool Empty() const {
        return size_ == 0;
    }

    bool Full() const {
        return size_ == Capacity;
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        size_ = 0;
        head_ = 0;
        is_front_ = true;
    }

    bool IsFront() const {
        if (size_ == 0) {
            is_front_ = true;
            head_ = Capacity;
        }
        return (!Full() && is_front_);
    }

    bool IsBack() const {
        if (size_ == 0) {
            is_front_ = false;
            head_ = 0;
        }
        return (!Full() && !(is_front_));
    }

    void PushBack(const T& value) {
        if (IsBack()) {
            buffer_[size_] = value;
            ++size_;
        }
    }

    void PushFront(const T& value) {
        if (IsFront()) {
            --head_;
            buffer_[head_] = value;
            ++size_;
        }
    }

    void PopBack() {
        --size_;
    }

    void PopFront() {
        ++head_;
        --size_;
    }
};

#endif // PAGE_H
