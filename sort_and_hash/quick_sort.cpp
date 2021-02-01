#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

template<class Iterator>
Iterator ChoosePivot(Iterator begin, Iterator end) {
    Iterator min = begin;
    Iterator mid = begin + (end - begin) / 2;
    Iterator max = end - 1;

    if (*min > *mid) {
        std::swap(min, mid);
    }
    if (*mid > *max) {
        std::swap(mid, max);
    }
    if (*min > *mid) {
        std::swap(min, mid);
    }
    return mid;
}

template<class Iterator>
Iterator Partition(Iterator begin, Iterator end, Iterator pivot) {
    std::swap(*pivot, *(end - 1));
    Iterator left = begin;
    Iterator right = end - 2;
    typename Iterator::value_type val = *(end - 1);

    while (left <= right) {
        while (*left < val) {
            ++left;
        }
        while (right >= begin && *right > val) {
            --right;
        }
        if (left >= right) {
            break;
        }
        std::swap(*left, *right);
        ++left;
        --right;
    }
    ++right;
    std::swap(*right, *(end - 1));
    return right;
}

template<class Iterator>
void QuickSort(Iterator begin, Iterator end) {
    if (end - begin <= 1) {
        return;
    }

    Iterator pivot = ChoosePivot(begin, end);
    Iterator middle = Partition(begin, end, pivot);
    QuickSort(begin, middle);
    QuickSort(middle + 1, end);
}

int main() {
    int size;
    std::cin >> size;

    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }

    QuickSort(arr.begin(), arr.end());

    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
