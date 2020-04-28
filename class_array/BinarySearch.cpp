#include <cstddef>
#include <utility>
#include <iostream>

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

template<class  T, size_t N>
void FillArray(Array<T, N>& arr, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
}

template <class T>
const T* GetMiddle(const T* left, const T* right) {
    return (left + (right - left) / 2);
}

template<class T>
bool BinarySearch(const T* begin, const T* end, const T& elem) {
    const T* left = begin;
    const T* right = end - 1;
    while (left != right) {
        const T* middle = GetMiddle(left, right);
        if (elem > *middle) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return (*left == elem);
}

int main() {
    const size_t kMaxLength = 100'000;
    size_t N, K;
    std::cin >> N >> K;

    Array<int, kMaxLength> first_array;
    FillArray(first_array, N);
    Array<int, kMaxLength> second_array;
    FillArray(second_array, K);

    for (size_t i = 0; i < K; ++i) {
        if (BinarySearch<int>(first_array.data(), first_array.data() + N,
                 second_array[i])) {
          std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    return 0;
}