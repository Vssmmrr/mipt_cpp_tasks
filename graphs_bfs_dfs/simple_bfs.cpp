#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int INF = 100000;

template <class T>
std::vector<int> GetShortestPath (const std::vector<std::vector<T>>& Graph,
                                  const int& start, const int& end) {
    std::queue<int> queue_of_vertexes;
    std::vector<int> distance(Graph.size(), INF), path(Graph.size());

    queue_of_vertexes.push(start);
    path[start] = -1;
    distance[start] = 0;
    while (!queue_of_vertexes.empty()) {
        int vertex = queue_of_vertexes.front();
        queue_of_vertexes.pop();
        for (const auto& neigbour : Graph[vertex]) {
            if (distance[neigbour] == INF) {
                queue_of_vertexes.push(neigbour);
                    distance[neigbour] = distance[vertex] + 1;
                    path[neigbour] = vertex;
            }
        }
    }

    std::vector<int> road;
    if (distance[end] == INF) {
        road.push_back(-1);
    } else {
        road.push_back(end);
        while (road.back() != start) {
            road.push_back(path[road.back()]);
        }
    }

    return {road.rbegin(), road.rend()};
}

int main() {
    int num_vertex, num_edge;
    std::cin >> num_vertex >> num_edge;

    int start, end;
    std::cin >> start >> end;
    --start;
    --end;

    std::vector<std::vector<int>> Graph(num_vertex);
    for (int i = 0; i < num_edge; ++i) {
        int first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        --first_vertex;
        --second_vertex;
        Graph[first_vertex].push_back(second_vertex);
        Graph[second_vertex].push_back(first_vertex);
    }

    std::vector<int> road = GetShortestPath(Graph, start, end);

    if (*road.begin() == -1) {
        std::cout << "-1\n";
    } else {
        std::cout << road.size() - 1 << '\n';
        for (const auto& elem : road) {
            std::cout << elem + 1 << ' ';
        }
    }

    return 0;
}

