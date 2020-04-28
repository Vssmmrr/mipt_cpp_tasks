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

template<class T>
struct Pointers{
    T* left_;
    T* right_;
};

template <class T>
T ChoosePivot(const T* left, const T* right) {
    return *(left + (right - left) / 2);
}

template <class T>
Pointers<T> Partition(T* begin, T* end, const T& pivot) {
    T* left = begin;
    T* right = end - 1;
    while (left <= right) {
        while (*left < pivot) {
            ++left;
        }
        while (*right > pivot) {
            --right;
        }
        if (left <= right) {
            std::swap(*left, *right);
            ++left;
            --right;
        }
    }
    return {left, right};
}

template <class T>
void QuickSort(T* begin, T* end) {
    T pivot = ChoosePivot(begin, end);
    Pointers<T> pointers = Partition(begin, end, pivot);
    if (begin < pointers.right_) {
        QuickSort(begin, pointers.right_ + 1);
    }
    if (end - 1 > pointers.left_) {
        QuickSort(pointers.left_, end);
    }
}

int main() {
    const size_t kMaxLength = 100'000;
    size_t N;
    std::cin >> N;
    Array<int, kMaxLength> arr;
    for (size_t i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }
    QuickSort(arr.Data(), arr.Data() + N);
    for (size_t i = 0; i < N; ++i) {
        std::cout << arr[i] << " ";
    }
    return 0;
}