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

template <class It, class It2, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
It2 Merge(It first_begin, It first_end, It second_begin, It second_end, It2 final, Cmp cmp = Cmp()) {
    while(first_begin != first_end && second_begin != second_end) {
        if (cmp(*second_begin, *first_begin)) {
            *final = *second_begin;
            ++final; ++second_begin;
        } else {
            *final = *first_begin;
            ++final; ++first_begin;
        }
    }
    final = std::copy(first_begin, first_end, final);
    final = std::copy(second_begin, second_end, final);
    return final;
}

template<class It, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
void MergeSort(It beg, It end, Cmp cmp, std::vector<typename std::iterator_traits<It>::value_type>& tmp) {
    if (end - beg <= 1) {
        return;
    }

    int size = end - beg;
    MergeSort(beg, beg + size / 2, cmp, tmp);
    MergeSort(beg + size / 2, end, cmp, tmp);
    Merge(beg, beg + size / 2, beg + size / 2, end, tmp.begin(), cmp);
    std::copy(tmp.begin(), tmp.begin() + size, beg);
}

template<class It, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
void MergeSort(It beg, It end, Cmp cmp = Cmp()) {
    std::vector<typename std::iterator_traits<It>::value_type> tmp(end - beg);
    MergeSort(beg, end, cmp, tmp);
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
