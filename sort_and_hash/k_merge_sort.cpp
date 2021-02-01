#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

template <class It, class It2, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
It2 Merge(It first_begin, It first_end, It second_begin, It second_end, It2 final, Cmp cmp = Cmp()) {
    while(first_begin != first_end && second_begin != second_end) {
        if (cmp(*first_begin, *second_begin)) {
            *final = *first_begin;
            ++final; ++first_begin;
        } else {
            *final = *second_begin;
            ++final; ++second_begin;
        }
    }
    final = std::copy(first_begin, first_end, final);
    final = std::copy(second_begin, second_end, final);
    return final;
}

template <class T>
int FinalSize(const std::vector<std::vector<T>>& array) {
    int final_size = 0;
    for (int i = 0; i < array.size(); ++i) {
        final_size += array[i].size();
    }
    return final_size;
}

template <class T>
std::vector<T> KMerge(std::vector<std::vector<T>> array) {
    int final_size = FinalSize(array);
    std::vector<T> tmp(final_size);

    for (int k = 1; k < array.size(); k *= 2) {
        for (int left = 0; left + k < array.size(); left += 2 * k) {
            Merge(array[left].begin(), array[left].end(),
                  array[left + k].begin(), array[left + k].end(), tmp.begin());
            array[left].resize(array[left].size() + array[left + k].size());
            std::copy(tmp.begin(), tmp.begin() + array[left].size(), array[left].begin());
        }
    }

    return array[0];
}

int main() {
    int num_array;
    std::cin >> num_array;

    std::vector<std::vector<int>> array(num_array);
    for (int i = 0; i < num_array; ++i) {
        int array_size;
        std::cin >> array_size;
        array[i].resize(array_size);
        for (int j = 0; j < array_size; ++j) {
            std::cin >> array[i][j];
        }
    }

    std::vector<int> final = KMerge(array);

    for (const auto& elem : final) {
        std::cout << elem << ' ';
    }

    return 0;
}
