#include <iostream>
#include <vector>

const int GetMiddle(const int left, const int right) {
    return (left + (right - left) / 2);
}

template<class Checker>
int BinarySearch(const int begin, const int end, Checker checker) {
    int left = begin;
    int right = end;
    while (left + 1 < right) {
        const int middle = GetMiddle(left, right);
        if (!checker(middle)) {
            right = middle;
        } else {
            left = middle;
        }
    }
    return left;
}

int main() {
    int num_desks, num_students;
    std::cin >> num_desks >> num_students;

    std::vector<int> coordinates(num_desks);
    for (int i = 0; i < num_desks; ++i) {
        std::cin >> coordinates[i];
    }

    auto distance_checker =
            [&coordinates, &num_students](int min_distance) -> bool {
       int students_left = num_students;
       int last_position = -min_distance;

       for (int i = 0; i < coordinates.size(); ++i) {
           if (coordinates[i] - last_position >= min_distance) {
               --students_left;
               last_position = coordinates[i];
               if (students_left == 0) {
                   return true;
               }
           }
       }
       return false;
    };

    int max_distance = BinarySearch(1, coordinates.back(), distance_checker);
    std::cout << max_distance << "\n";

    return 0;
}


