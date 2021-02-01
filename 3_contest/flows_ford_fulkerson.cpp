#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

struct Edge {
    size_t from_;
    size_t to_;
    int64_t flow_ = 0;
    size_t back_index_ = 0;
    int64_t capacity_;

    Edge() = default;
    Edge(size_t from, size_t to, int64_t capacity) : from_(from), to_(to), capacity_(capacity) {
    }
};

typedef size_t vertex_t;
typedef int64_t weight_t;
typedef std::vector<std::vector<Edge>> Graph;

const int64_t INF = 10000000;

struct DfsStatus {
    std::vector<vertex_t> pred_;
    std::vector<size_t> pred_edge_number_;
    std::vector<bool> visited_;
};

weight_t FindFlow(Graph& graph, vertex_t start, vertex_t end, DfsStatus& status) {
    status.visited_[start] = true;
    if (start == end) {
        return INF;
    }
    size_t edge_no = 0;
    for (auto& cur_edge : graph[start]) {
        if (!status.visited_[cur_edge.to_] && (cur_edge.capacity_ - cur_edge.flow_) > 0) {
            status.pred_[cur_edge.to_] = start;
            status.pred_edge_number_[cur_edge.to_] = edge_no;
            int64_t flow = cur_edge.capacity_ - cur_edge.flow_;
            flow = std::min(flow, FindFlow(graph, cur_edge.to_, end, status));
            if (flow > 0) {
                return flow;
            }
        }
        ++edge_no;
    }
    return 0;
}

void UpdateGraph(Graph& graph, vertex_t start, vertex_t end, weight_t flow, const DfsStatus& status) {
    while (start != end) {
        vertex_t prev = status.pred_[end];
        Edge& cur_edge = graph[prev][status.pred_edge_number_[end]];
        Edge& back_edge = graph[end][cur_edge.back_index_];

        cur_edge.flow_ += flow;
        back_edge.flow_ -= flow;

        end = prev;
    }
}

weight_t MaxFlow(Graph graph, vertex_t start, vertex_t end) {
    int64_t current_flow = 0;

    while (true) {
        DfsStatus status;
        status.pred_.resize(graph.size(), -1);
        status.pred_edge_number_.resize(graph.size(), -1);
        status.visited_.resize(graph.size(), false);
        auto flow = FindFlow(graph, start, end, status);
        if (flow == 0) {
            return current_flow;
        }
        current_flow += flow;
        UpdateGraph(graph, start, end, flow, status);
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertex, num_edges;
    std::cin >> num_vertex >> num_edges;
    size_t start = 0;
    size_t end = num_vertex - 1;

    Graph list_graph(num_vertex);
    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        int weight;
        std::cin >> from >> to >> weight;
        list_graph[from - 1].push_back({from - 1, to - 1, weight});
        list_graph[to - 1].push_back({to - 1, from - 1, 0});

        list_graph[from - 1].back().back_index_ = list_graph[to - 1].size() - 1;
        list_graph[to - 1].back().back_index_ = list_graph[from - 1].size() - 1;
    }

    std::cout << MaxFlow(list_graph, start, end) << "\n";

    return 0;
}

