#include <vector>
#include <iostream>

ssize_t kInfinity = 1000000000;

template <class Iterator>
struct Subarray {
    Iterator begin;
    Iterator end;
    ssize_t sum;

    bool operator<(const Subarray& other) const {
        return sum < other.sum;
    }
};

template <class Iterator>
Subarray<Iterator> FindMaximumSubarrayCrossingMid(Iterator begin, Iterator end, Iterator mid) {
    ssize_t left_sum = 0;
    Iterator left = mid;
    ssize_t cur_sum = 0;
    for (Iterator it = mid - 1; it >= begin; --it) {
        cur_sum += *it;
        if (cur_sum > left_sum) {
            left_sum = cur_sum;
            left = it;
        }
    }

    ssize_t right_sum = 0;
    Iterator right = mid;
    cur_sum = 0;
    for (Iterator it = mid + 1; it < end; ++it) {
        cur_sum += *it;
        if (cur_sum > right_sum) {
            right_sum = cur_sum;
            right = it;
        }
    }

    return {left, right + 1, left_sum + right_sum + *mid};
}

template <class Iterator>
Subarray<Iterator> FindMaximumSubarray(Iterator begin, Iterator end) {
    if (begin + 1 == end) {
        return {begin, end, *begin};
    } else {
        Iterator mid = begin + (end - begin) / 2;

        auto left_subarray = FindMaximumSubarray(begin, mid);
        auto right_subarray = FindMaximumSubarray(mid, end);
        auto crossing_subarray = FindMaximumSubarrayCrossingMid(begin, end, mid);

        return std::max(left_subarray, std::max(right_subarray, crossing_subarray));
    }
}

int main() {
    size_t size;
    std::cin >> size;
    std::vector<ssize_t> array(size);
    for (size_t i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    auto subarray = FindMaximumSubarray(array.begin(), array.end());
    std::cout << subarray.begin - array.begin() + 1 << " " << subarray.end - array.begin() << " " << subarray.sum << "\n";

    return 0;
}
