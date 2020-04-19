#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <utility>

template <class T>
inline void Fill(T* begin, T* end, const T& value) {
    for (T* ptr = begin; ptr < end; ++ptr) {
        *ptr = value;
    }
}

template  <class T>
inline void Copy(const T* source, T* destination, size_t copy_size) {
    for(size_t i = 0; i < copy_size; ++i) {
        destination[i] = source[i];
    }
}

template <class T>
class Vector {
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    T* buffer_ = nullptr;

    const static size_t kIncreaseFactor = 2;

    size_t CalculateCapacity(size_t expected_capacity) {
        size_t new_capacity = capacity_;
        if (new_capacity == 0) {
            new_capacity= 1;
        }
        while (new_capacity < expected_capacity) {
            new_capacity *= kIncreaseFactor;
        }
        return new_capacity;
    }

    void Reallocate(size_t expected_capacity) {
        capacity_ = expected_capacity;
        T* tmp = new T[capacity_];
        Copy(buffer_,tmp, size_);
        delete[] buffer_;
        buffer_ = tmp;
    }

public:
    Vector() : size_(0), capacity_(0), buffer_(nullptr) {
    }

    explicit Vector(size_t size) : size_(size), capacity_ (size) {
        buffer_ = new T[capacity_];
        Fill(buffer_, buffer_ + size_, T());
    }

    Vector(size_t size, const T& value) : size_(size), capacity_ (size) {
        buffer_ = new T[capacity_];
        Fill(buffer_, buffer_ + size_, value);
    }

    Vector(const Vector& other) : Vector(other.size_) {
        Copy(other.buffer_, buffer_,  size_);
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        Reallocate(other.size_);
        size_ = other.size_;
        Copy(other.buffer_, buffer_, size_);
        return *this;
    }

    ~Vector() {
        delete[] buffer_;
    }

    void Clear() {
        size_ = 0;
    }

    void PushBack(const T& value) {
        size_t expected_cap = CalculateCapacity(size_ + 1);
        if (capacity_ != expected_cap) {
            capacity_ = expected_cap;
            Reallocate(capacity_);
        }
        buffer_[size_] = value;
        ++size_;
    }

    void PopBack() {
        if (size_ > 0) {
            --size_;
        }
    }

    void Resize(const size_t new_size) {
        Resize(new_size, T());
    }

    void Resize(const size_t new_size, const T& value) {
        if (new_size <= size_) {
            size_ = new_size;
        } else {
            capacity_ = CalculateCapacity(new_size);
            Reallocate(capacity_);
            Fill(buffer_ + size_, buffer_ + new_size, value);
            size_ = new_size;
        }
    }

    void Reserve(size_t new_cap) {
        if (new_cap <= capacity_) {
            return;
        }
        Reallocate(new_cap);
    }

    void ShrinkToFit() {
        if (capacity_ == size_) {
            return;
        }
        Reallocate(size_);
    }

    void Swap(Vector& other) {
        std::swap(buffer_, other.buffer_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    T operator[](size_t idx) const {
        return buffer_[idx];
    }

    T& operator[](size_t idx) {
        return buffer_[idx];
    }

    T Front() const {
        return buffer_[0];
    }

    T& Front() {
        return buffer_[0];
    }

    T Back() const {
        return buffer_[size_ - 1];
    }

    T& Back() {
        return buffer_[size_ - 1];
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    T* Data() {
        return buffer_;
    }

    const T* Data() const {
        return buffer_;
    }
};



template <class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    for (size_t i = 0; i < lhs.Size() && i < rhs.Size(); ++i) {
        if (lhs[i] > rhs[i]) {
            return true;
        } else if (lhs[i] < rhs[i]) {
            return false;
        }
    }
    return lhs.Size() > rhs.Size();
}

template <class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs > lhs;
}

template <class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs > rhs);
}

template  <class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.Size() != rhs.Size()) {
        return false;
    } else {
        for (size_t i = 0; i < lhs.Size(); ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }
}

template  <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

#endif // VECTOR_H
