#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_map>

typedef size_t vertex_t;
typedef std::vector<std::vector<std::pair<vertex_t, int>>> Graph;
const int INF = 100000;
const int kMaxLength = 1;


int GetShortestPath(const Graph& graph, vertex_t start, vertex_t end) {
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

void CheckEdge(Graph& graph, vertex_t from, vertex_t to, bool forward_edge = true) {
    int weight;

    if (forward_edge) {
        weight = kMaxLength - 1;
    } else {
        weight = kMaxLength;
    }

    bool has_edge = false;
    for (auto &elem: graph[from]) {
        if (elem.first == to) {
            elem.second = std::min(weight, elem.second);
            has_edge = true;
            break;
        }
    }

    if (!has_edge) {
        graph[from].push_back({to, weight});
    }
}

Graph GraphFromAdjMatrixAndEdgeList(const std::vector<std::vector<bool>>& adj_matrix,
                                    const std::vector<std::pair<vertex_t, vertex_t>>& edge_list) {
    Graph graph(adj_matrix.size());
    std::vector<std::vector<bool>> edge_added(adj_matrix.size(),
                                              std::vector<bool>(adj_matrix.size(), false));

    for (const auto [from, to] : edge_list) {
        if (edge_added[from][to]) {
            continue;;
        }
        if (adj_matrix[from][to]) {
            graph[from].push_back({to, 0});
        }

        if (adj_matrix[to][from]) {
            graph[to].push_back({from, 0});
        } else {
            graph[to].push_back({from, 1});
        }

        edge_added[from][to] = true;
    }
    return graph;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertices, num_edges;
    std::cin >> num_vertices >> num_edges;
    std::vector<std::vector<bool>> adjacency_matrix(num_vertices, std::vector<bool>(num_vertices, false));
    std::vector<std::pair<vertex_t, vertex_t>> edge_list;

    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        std::cin >> from >> to;
        if (from == to) {
            continue;
        }
        // CheckEdge(graph, from - 1, to - 1);
        // CheckEdge(graph, to - 1, from - 1, false);
        adjacency_matrix[from - 1][to - 1] = true;
        edge_list.push_back({from - 1, to - 1});
        //edge_list.push_back({to - 1, from - 1});
    }

    Graph graph(GraphFromAdjMatrixAndEdgeList(adjacency_matrix, edge_list));

    size_t k;
    std::cin >> k;
    for (size_t i = 0; i < k; ++i) {
        vertex_t start, end;
        std::cin >> start >> end;
        int shortest_path = GetShortestPath(graph, start - 1, end - 1);
        std::cout << shortest_path << '\n';
    }

    return 0;
}
