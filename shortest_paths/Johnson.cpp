#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

typedef size_t vertex_t;

const int kInfinity = 2009000999;

struct Edge {
    vertex_t from;
    vertex_t to;
    int64_t weight;
};

typedef std::vector<std::vector<Edge>> Graph;

bool FordBellmanRelax(const Edge edge, std::vector<int64_t>& min_dist) {
    if (min_dist[edge.to] > min_dist[edge.from] + edge.weight) {
        min_dist[edge.to] = min_dist[edge.from] + edge.weight;
        return true; // did relax
    }
    return false; // didn't relax
}

bool CheckNegativeCycle(const Graph& graph, std::vector<int64_t>& min_distance) {
    const size_t num_vertices = graph.size();
    for (size_t i = 0; i < num_vertices - 1; ++i) {
        for (const auto& edge_list : graph) {
            for (const auto &edge : edge_list) {
                FordBellmanRelax(edge, min_distance);
            }
        }
    }

    for (const auto& edge_list : graph) {
        for (const auto &edge : edge_list) {
            if (FordBellmanRelax(edge, min_distance)) { // has cycle
                return true;
            }
        }
    }

    return false;
}

bool DijkstraRelax(const Edge& edge, std::vector<int64_t>& min_dist, std::vector<vertex_t>& prev) {
    if (min_dist[edge.to] == kInfinity || min_dist[edge.to] > min_dist[edge.from] + edge.weight) {
        min_dist[edge.to] = min_dist[edge.from] + edge.weight;
        prev[edge.to] = edge.from;
        return true; // did relax
    }
    return false; // didn't relax
}

std::vector<int64_t> GetDistances(const Graph& graph, vertex_t start) {
    uint64_t graph_size = graph.size();
    std::vector<bool> visited(graph_size, false);
    std::vector<vertex_t> previous(graph_size, -1);
    std::vector<int64_t> distances(graph_size, kInfinity);

    distances[start] = 0;
    std::priority_queue<std::pair<int64_t, vertex_t>, std::vector< std::pair<int64_t, vertex_t>>, std::greater<std::pair<int64_t, vertex_t>>> distances_heap;
    distances_heap.push({ distances[start], start });
    while (!distances_heap.empty()) {
        auto current_vertex = distances_heap.top().second;
        auto cur_vert_dist = distances_heap.top().first;
        distances_heap.pop();

        if (!visited[current_vertex]) {
            for (const auto& edge : graph[current_vertex]) {
                if (DijkstraRelax(edge, distances, previous)) {
                    distances_heap.push({ distances[edge.to], edge.to });
                }
            }
            visited[current_vertex] = true;
        }
    }
    return distances;
}

std::vector<std::vector<int64_t>> GetDistMatrix(Graph graph) {
    const size_t num_vertices = graph.size();

    std::vector<int64_t> distances_from_stock(num_vertices, 0);
    if (CheckNegativeCycle(graph, distances_from_stock)) {
        return {};
    }

    for (auto& edge_list : graph) {
        for (auto &edge : edge_list) {
            edge.weight += distances_from_stock[edge.from] - distances_from_stock[edge.to];
        }
    }

    std::vector<std::vector<int64_t>> distances(num_vertices);
    for (size_t i = 0; i < num_vertices; ++i) {
        distances[i] = GetDistances(graph, i);
    }
    return distances;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertices;
    std::cin >> num_vertices;
    Graph graph(num_vertices);
    for (size_t i = 0; i < num_vertices; ++i) {
        for (size_t j = 0; j < num_vertices; ++j) {
            int64_t weight;
            std::cin >> weight;

            if (weight != -1 && i != j) {
                graph[i].push_back({i, j, weight});
            }
        }
    }

    auto dist = GetDistMatrix(graph);
    int64_t max_dist = 0;
    for (size_t i = 0; i < num_vertices; ++i) {
        for (size_t j = 0; j < num_vertices; ++j) {
            if (dist[i][j] != kInfinity) {
                max_dist = std::max(max_dist, dist[i][j]);
            }
        }
    }
    std::cout << max_dist << "\n";
}

