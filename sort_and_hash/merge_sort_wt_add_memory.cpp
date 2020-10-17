#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

template <class Iterator>
void InplaceMerge(Iterator first_begin, Iterator first_end, Iterator second_begin, Iterator second_end) {
    int first_part_size = first_end - first_begin;
    int second_part_size = second_end - second_begin;

    if ((first_part_size == 0) || (second_part_size == 0)) {
        return;
    } else if ((first_part_size == 1) && (second_part_size == 1)) {
        if (*first_begin > *second_begin) {
            std::swap(*first_begin, *second_begin);
        }
        return;
    }

    Iterator B1_beg = first_begin;
    Iterator B2_end = first_end;
    Iterator B1_end, B2_beg;
    Iterator B3_beg = second_begin;
    Iterator B4_end = second_end;
    Iterator B3_end, B4_beg;

    if (first_part_size >= second_part_size) {
        B1_end = B1_beg + first_part_size / 2;
        B2_beg = B1_end;

        typename Iterator::value_type val = *B2_beg;
        Iterator pivot = std::lower_bound(second_begin, second_end, val);
        B3_end = pivot;
        B4_beg = pivot;
    } else {
        B3_end = B3_beg + second_part_size / 2;
        B4_beg = B3_end;

        typename Iterator::value_type val = *B4_beg;
        Iterator pivot = std::upper_bound(first_begin, first_end, val);
        B1_end = pivot;
        B2_beg = pivot;
    }
    int B3_size = B3_end - B3_beg;

    std::rotate(B2_beg, B3_beg, B3_end);

    InplaceMerge(B1_beg, B1_end, B2_beg, B2_beg + B3_size);
    InplaceMerge(B2_beg + B3_size, B3_end, B4_beg, B4_end);
}


int main() {
//    std::ios_base::sync_with_stdio(false);
//    std::cin.tie(nullptr);

    int first_size;
    std::cin >> first_size;

    std::vector<int> first_array(first_size);
    for (int i = 0; i < first_size; ++i) {
        std::cin >> first_array[i];
    }

    int second_size;
    std::cin >> second_size;

    first_array.resize(first_size + second_size);
    for (int i = 0; i < second_size; ++i) {
        std::cin >> first_array[first_size + i];
    }

    InplaceMerge(
            first_array.begin(), first_array.begin() + first_size,
            first_array.begin() + first_size, first_array.end()
    );

    for (int i = 0; i < first_size + second_size; ++i) {
        std::cout << first_array[i] << " ";
    }

    return 0;
}
