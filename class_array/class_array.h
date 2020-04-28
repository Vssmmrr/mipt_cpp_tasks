#include <cstddef>
#include <utility>
#ifndef CLASS_ARRAY_H
#define CLASS_ARRAY_H

template <class T, size_t N>
class Array {
    T buffer_[N];
public:
    Array() = default;
    Array(const Array& other) = default;
    ~Array() = default;

    T& operator[](size_t idx) {
        return buffer_[idx];
    }

    T operator[](size_t idx) const {
        return buffer_[idx];
    }

    T& Front() {
        return buffer_[0];
    }

    T& Back() {
        return buffer_[N - 1];
    }

    T Front() const {
        return buffer_[0];
    }

    T Back() const {
        return buffer_[N - 1];
    }

    T* Data() {
        return buffer_;
    }

    const T* data() const {
        return buffer_;
    }

    bool Empty() const {
        return N == 0;
    }

    size_t Size() const {
        return N;
    }

    void Fill(const T& value) {
        for(size_t i = 0; i < N; ++i) {
            buffer_[i] = value;
        }
    }

    void Swap(Array& other) {
        std::swap(buffer_, other.buffer_);
    }
};

template<size_t I, size_t N, class T>
T& Get(Array<T, N>& arr) {
    return arr[I];
}

template<size_t I, size_t N, class T>
const T& Get(const Array<T, N>& arr) {
    return arr[I];
}

template<class T, size_t N>
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    for(size_t i = 0; i < lhs.Size() && i < rhs.Size(); ++i) {
        if (lhs[i] > rhs[i]) {
            return true;
        } else if (lhs[i] < rhs[i]) {
            return false;
        }
    }
    return false;
}

template<class T, size_t N>
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return (rhs > lhs);
}

template<class T, size_t N>
bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs > rhs);
}

template<class T, size_t N>
bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs < rhs);
}

template<class T, size_t N>
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) {
//    if (lhs.Size() != rhs.Size()) {
//        return false;
//    }
    for (size_t i = 0; i < lhs.Size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template<class T, size_t N>
bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs == rhs);
}

#endif //CLASS_ARRAY_H

 // void* operator new(size_t size);
 // void* operator new[](size_t size);