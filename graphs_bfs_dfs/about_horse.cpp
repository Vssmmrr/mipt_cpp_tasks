#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int INF = 100000;

std::vector<std::pair<int, int>> FillInNeighbours(const int graph_size, const int field_x, const int field_y) {
    int moves_x[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int moves_y[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    std::vector<std::pair<int, int>> neighbours;

    for (int i = 0; i < 8; ++i) {
        int new_x = moves_x[i] + field_x;
        int new_y = moves_y[i] + field_y;
        if (new_x >= 0 && new_x < graph_size && new_y >= 0 && new_y < graph_size) {
            neighbours.push_back({new_x, new_y});
        }
    }
    return neighbours;
}

std::vector<std::pair<int, int>> GetShortestPathToTrava(const int graph_size,
                                  std::pair<int, int> start, std::pair<int, int> end) {
    std::queue<std::pair<int, int>> queue_of_vertexes;
    std::vector<std::vector<int>> distance(graph_size, std::vector<int>(graph_size, INF));
    std::vector<std::vector<std::pair<int, int>>> previous(graph_size,
               std::vector<std::pair<int, int>>(graph_size, {-1, -1}));

    queue_of_vertexes.push(start);
    distance[start.second][start.first] = 0;
    while (!queue_of_vertexes.empty()) {
        auto [x, y] = queue_of_vertexes.front();
        queue_of_vertexes.pop();

        auto neighbours = FillInNeighbours(graph_size, x, y);
        for (auto [new_x, new_y] : neighbours) {
            if (distance[new_y][new_x] == INF) {
                distance[new_y][new_x] = distance[y][x] + 1;
                previous[new_y][new_x] = {x, y};
                queue_of_vertexes.push({new_x, new_y});
            }
        }
    }

    std::vector<std::pair<int, int>> road;
    if (distance[end.second][end.first] == INF) {
        return {};
    }
    std::pair<int, int> current = end;
    while (current != start) {
        road.push_back(current);
        current = previous[current.second][current.first];
    }
    road.push_back(start);

    return {road.rbegin(), road.rend()};
}

int main() {
    int num_cells, x1, y1, x2, y2;
    std::cin >> num_cells >> x1 >> y1 >> x2 >> y2;

    std::pair<int, int> start, end;
    start = std::make_pair(x1 - 1, y1 - 1);
    end = std::make_pair(x2 - 1, y2 - 1);

    auto road = GetShortestPathToTrava(num_cells, start, end);
    std::cout << road.size() - 1 << std::endl;
    for (auto [x, y] : road) {
        std::cout << x + 1 << " " << y + 1 << "\n";
    }

    return 0;
}

