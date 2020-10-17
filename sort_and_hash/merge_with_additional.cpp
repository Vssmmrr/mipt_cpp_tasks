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
    while (first_begin != first_end) {
        *final = *first_begin;
        ++final; ++first_begin;
    }
    while (second_begin != second_end) {
        *final = *second_begin;
        ++final; ++second_begin;
    }
    return final;
}

int main() {
    int first_size;
    std::cin >> first_size;

    std::vector<int> first_array(first_size);
    for (int i = 0; i < first_size; ++i) {
        std::cin >> first_array[i];
    }

    int second_size;
    std::cin >> second_size;

    std::vector<int> second_array(second_size);
    for (int i = 0; i < second_size; ++i) {
        std::cin >> second_array[i];
    }

    std::vector<int> final_array(first_size + second_size);
    Merge(first_array.begin(), first_array.end(), second_array.begin(),
            second_array.end(), final_array.begin());

    for (const auto& elem : final_array) {
        std::cout << elem << ' ';
    }

    return 0;
}
