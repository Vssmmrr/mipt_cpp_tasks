#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

template <class T>
class UniquePtr {
private:
    T* ptr_;

public:
    UniquePtr() : ptr_(nullptr) {
    }

    explicit UniquePtr(T* ptr) : ptr_(ptr) {
    }

    ~UniquePtr() {
        delete ptr_;
    }

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (this == &other) {
            return *this;
        }
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    T* Get() const {
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T* Release() {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void Reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
    }

    void Swap(UniquePtr<T>& other) {
        std::swap(ptr_, other);
    }

};

//#define MAKE_UNIQUE_IMPLEMENTED

#endif // UNIQUE_PTR_H

