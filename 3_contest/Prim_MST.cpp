#include <iostream>
#include <queue>
#include <vector>

typedef size_t vertex_t;
typedef std::vector<std::vector<std::pair<vertex_t, int>>> Graph;
const int INF = 1000000;

struct Edge {
    size_t from_;
    size_t to_;
    int weight_;

    Edge() = default;
    Edge(size_t from, size_t to, int weight) : from_(from), to_(to), weight_(weight) {
    }
};

bool operator>(const Edge& lhs, const Edge& rhs) {
    return lhs.weight_ > rhs.weight_;
}

int Prim(const Graph& graph) {
    size_t num_vertices = graph.size();
    std::vector<int> d(num_vertices, INF);
    std::vector<bool> used(num_vertices, false);
    std::vector<Edge> answer;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> q;

    vertex_t current = 0;
    used[0] = true;
    for (size_t i = 0; i < graph[current].size(); ++i) {
        q.push({current, graph[current][i].first, graph[current][i].second});
    }
    while (answer.size() < (num_vertices - 1)) {
        Edge current_edge = q.top();
        q.pop();
        if (used[current_edge.to_]) {
            continue;
        }
        used[current_edge.to_] = true;
        answer.push_back(current_edge);
        current = current_edge.to_;
        for (const auto elem : graph[current]) {
            if (!used[elem.first] && d[elem.first] > elem.second) {
                d[elem.first] = elem.second;
                q.push({current, elem.first, elem.second});
            }
        }
    }

    int min_weight_span_tree = 0;
    for (const auto& elem : answer) {
        min_weight_span_tree += elem.weight_;
    }
    return min_weight_span_tree;
}

int main() {
    size_t num_vertices, num_edges;
    std::cin >> num_vertices >> num_edges;

    Graph graph(num_vertices);
    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        int weight;
        std::cin >> from >> to >> weight;
        graph[from - 1].push_back({to - 1, weight});
        graph[to - 1].push_back({from - 1, weight});
    }

    std::cout << Prim(graph) << '\n';

    return 0;
}

