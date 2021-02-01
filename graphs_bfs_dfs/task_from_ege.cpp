#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

const int INF = 100000;


std::vector<int> FillInVariantsNumbers(const int number) {
    std::vector<int> variants;
    int first_num = number / 1000;
    int last_num = number % 10;
    int second_num = (number / 100) % 10;
    int third_num = (number / 10) % 10;
    if (first_num != 9) {
        int ost = number % 1000;
        int new_num = (first_num + 1) * 1000 + ost;
        variants.push_back(new_num);
    }
    if (last_num != 1) {
        variants.push_back(number - 1);
    }
    variants.push_back(last_num * 1000 + first_num * 100 + second_num * 10 + third_num);
    variants.push_back(second_num * 1000 + third_num * 100 + last_num * 10 + first_num);
    return variants;
}

std::vector<int> GetShortestOperationsPath (const int first, const int second) {
    std::queue<int> queue_of_numbers;
    std::unordered_map<int, int> distance;
    std::unordered_map<int, int> previous;
    queue_of_numbers.push(first);
    distance[first] = 0;
    while (!queue_of_numbers.empty()) {
        int current_num = queue_of_numbers.front();
        queue_of_numbers.pop();

        auto next_numbers = FillInVariantsNumbers(current_num);
        for (const auto& elem : next_numbers) {
            if (distance.find(elem) == distance.end()) {
                distance[elem] = distance[current_num] + 1;
                previous[elem] = current_num;
                queue_of_numbers.push(elem);
            }
        }
    }

    std::vector<int> road;
    if (distance.find(second) == distance.end()) {
        return {};
    }
    int current = second;
    while (current != first) {
        road.push_back(current);
        current = previous[current];
    }
    road.push_back(first);

    return {road.rbegin(), road.rend()};
}

int main() {
    int first_num, second_num;
    std::cin >> first_num >> second_num;

    std::vector<int> path = GetShortestOperationsPath(first_num, second_num);
    std::cout << path.size() << '\n';
    for (const auto& elem : path) {
        std::cout << elem << '\n';
    }

    return 0;
}


