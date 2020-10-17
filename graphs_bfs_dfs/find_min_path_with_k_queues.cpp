#include <iostream>
#include <vector>
#include <queue>
#include <utility>

typedef size_t vertex_t;
typedef std::vector<std::vector<std::pair<vertex_t, int>>> Graph;
const int INF = 100000;



int GetShortestPath(const Graph& graph, vertex_t start, vertex_t end, int kMaxLength) {
    size_t num_vertices = graph.size();
    std::vector<bool> visited(num_vertices, false);
    std::vector<std::queue<vertex_t>> array_of_queues(kMaxLength + 1);
    array_of_queues[0].push(start);
    const size_t max_distance = kMaxLength * (num_vertices - 1);
    for (size_t distance = 0; distance < max_distance; ++distance) {
        size_t id = distance % (kMaxLength + 1);
        if (!array_of_queues[id].empty()) {
            while (!array_of_queues[id].empty()) {
                vertex_t v = array_of_queues[id].front();
                array_of_queues[id].pop();
                if (visited[v]) {
                    continue;
                }
                if (v == end) {
                    return distance;
                }
                auto neighbours = graph[v];
                for (const auto& elem : neighbours) {
                    array_of_queues[(id + elem.second) % (kMaxLength + 1)].push(elem.first);
                }

                visited[v] = true;
            }
        }
    }

    return -1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertices, num_edges, start, end;;
    std::cin >> num_vertices >> num_edges >> start >> end;
    Graph graph(num_vertices);
    int kMaxLength = 0;
    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        int length;
        std::cin >> from >> to >> length;
        if (length > kMaxLength) {
            kMaxLength = length;
        }
        graph[from - 1].push_back(std::make_pair(to - 1, length));
    }

    int shortest_path = GetShortestPath(graph, start - 1, end - 1, kMaxLength);
    std::cout << shortest_path << '\n';

    return 0;
}
