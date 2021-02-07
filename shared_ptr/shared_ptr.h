#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <utility>

class BadWeakPtr {

};


template <class T>
class SharedPtr {
private:
    T* ptr_;
    static int cnt_;
public:
    SharedPtr() : ptr_(nullptr) {
    }

    SharedPtr(T* ptr) : ptr_(ptr) {
    }

    SharedPtr(const SharedPtr& other) {
        ptr_ = new T;
        ptr_ = other.ptr_;
        ++cnt_;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        delete ptr_;
        ptr_ = new T;
        ptr_ = other.ptr_;
        ++cnt_;
        return *this;
    }

    SharedPtr(SharedPtr&& other) {
        ptr_ = new T;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (this == &other) {
            return *this;
        }
        delete ptr_;
        ptr_ = new T;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    ~SharedPtr() {
        if (cnt_ == 1) {
            delete ptr_;
        } else {
            ptr_ = nullptr;
            --cnt_;
        }
    }

//            Конструктор от WeakPtr (детали ниже)

    void Reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
    }

    void Swap(SharedPtr<T>& other) {
        std::swap(ptr_, other);
        std::swap(cnt_, other.cnt_);
    }

    T* Get() const {
        return ptr_;
    }

    int UseCount() const {
        return cnt_;
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

};

template <class T>
class WeakPtr {

};


//#define MS_IMPLEMENTED


#endif // SHARED_PTR_H
