#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>


struct Person {
    std::string surname_;
    std::string name_;
    int inf_points_;
    int math_points_;
    int rus_points_;
    double mean_;
};

template <class Iterator, class Cmp = std::less<typename std::iterator_traits<Iterator>::value_type>>
void InplaceMerge(Iterator first_begin, Iterator first_end, Iterator second_begin, Iterator second_end, Cmp cmp = Cmp()) {
    int first_part_size = first_end - first_begin;
    int second_part_size = second_end - second_begin;

    if ((first_part_size == 0) || (second_part_size == 0)) {
        return;
    } else if ((first_part_size == 1) && (second_part_size == 1)) {
        if (cmp(*second_begin, *first_begin)) {
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
        Iterator pivot = std::lower_bound(second_begin, second_end, val, cmp);
        B3_end = pivot;
        B4_beg = pivot;
    } else {
        B3_end = B3_beg + second_part_size / 2;
        B4_beg = B3_end;

        typename Iterator::value_type val = *B4_beg;
        Iterator pivot = std::upper_bound(first_begin, first_end, val, cmp);
        B1_end = pivot;
        B2_beg = pivot;
    }
    int B3_size = B3_end - B3_beg;

    std::rotate(B2_beg, B3_beg, B3_end);

    InplaceMerge(B1_beg, B1_end, B2_beg, B2_beg + B3_size, cmp);
    InplaceMerge(B2_beg + B3_size, B3_end, B4_beg, B4_end, cmp);
}

template<class It, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
void MergeSort(It beg, It end, Cmp cmp) {
    if (end - beg <= 1) {
        return;
    }

    int size = end - beg;
    MergeSort(beg, beg + size / 2, cmp);
    MergeSort(beg + size / 2, end, cmp);
    InplaceMerge(beg, beg + size / 2, beg + size / 2, end, cmp);
}

int main() {
    int number;
    std::cin >> number;

    std::vector<Person> people(number);
    for (int i = 0; i < number; ++i) {
        std::cin >> people[i].surname_;
        std::cin >> people[i].name_;
        std::cin >> people[i].inf_points_;
        std::cin >> people[i].math_points_;
        std::cin >> people[i].rus_points_;
        people[i].mean_ = (double)(people[i].inf_points_ + people[i].math_points_ + people[i].rus_points_) / 3;
    }

    auto cmp = [](Person lhs, Person rhs) {return lhs.mean_ > rhs.mean_;};
    MergeSort(people.begin(), people.end(), cmp);

    for (const auto& elem : people) {
        std::cout << elem.surname_ << ' ' << elem.name_ << '\n';
    }

    return 0;
}

