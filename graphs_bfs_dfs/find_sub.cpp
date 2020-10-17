#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

typedef std::vector<std::vector<int>> Map;
typedef std::pair<int, int> coordinates;
const int INF = 100000;


Map GetDistancesToRestaurants(int first_city_parameter, int second_city_parameter,
                              std::vector<coordinates> restaurants) {
    Map distances(first_city_parameter, std::vector<int>(second_city_parameter, INF));
    std::queue<coordinates> order;
    std::vector<coordinates> moves = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };

    for (const auto& elem : restaurants) {
        order.push(elem);
        distances[elem.first][elem.second] = 0;
    }
    while (!order.empty()) {
        coordinates current = order.front();
        order.pop();
        for (auto move : moves) {
            coordinates destination{current.first + move.first, current.second + move.second};
            if (destination.first < 0 || destination.first >= first_city_parameter ||
                destination.second < 0 || destination.second >= second_city_parameter) {
                continue;
            }
            if (distances[destination.first][destination.second] == INF) {
                order.push(destination);
                distances[destination.first][destination.second] = distances[current.first][current.second] + 1;
            }
        }
    }
    return distances;
}


int main() {
    int first_city_parameter, second_city_parameter;
    std::cin >> first_city_parameter >> second_city_parameter;
    std::vector<coordinates> restaurants;
    for (int i = 0; i < first_city_parameter; ++i) {
        for (int j = 0; j < second_city_parameter; ++j) {
            int value;
            std::cin >> value;
            if (value == 1) {
                restaurants.push_back({i, j});
            }
        }
    }

    auto distances = GetDistancesToRestaurants(first_city_parameter, second_city_parameter, restaurants);

    for (int i = 0; i < first_city_parameter; ++i) {
        for (int j = 0; j < second_city_parameter; ++j) {
            std::cout << distances[i][j] << " ";
        }
        std::cout << '\n';
    }


    return 0;
}

