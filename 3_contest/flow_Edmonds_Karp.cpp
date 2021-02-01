#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <queue>

struct Edge {
	size_t from;
	size_t to;
	int64_t flow = 0;
	size_t back_index = 0;
	int64_t capacity;

	Edge() = default;
	Edge(size_t from, size_t to, int64_t capacity) : from(from), to(to), capacity(capacity) {
	}
};

typedef size_t vertex_t;
typedef int64_t weight_t;
typedef std::vector<std::vector<Edge>> Graph;

const int64_t kInfinity = 10000000;

struct DfsStatus {
	std::vector<vertex_t> pred;
	std::vector<size_t> pred_edge_number;
	std::vector<bool> visited;

	DfsStatus(size_t size) : pred(size, -1), pred_edge_number(size, -1), visited(size, false) {
	}
};

weight_t FindFlow(const Graph& graph, vertex_t start, vertex_t end, DfsStatus& status) {
	std::queue<vertex_t> vertices;
	std::vector<weight_t> flow(graph.size(), 0);

	vertices.push(start);
	flow[start] = kInfinity;

	while (!vertices.empty()) {
		auto vert = vertices.front();
		vertices.pop();

		status.visited[vert] = true;

		size_t edge_no = 0;
		for (const auto& edge : graph[vert]) {
			weight_t edge_flow = std::min(edge.capacity - edge.flow, flow[vert]);
			if (flow[edge.to] < edge_flow && !status.visited[edge.to]) {
				status.pred[edge.to] = vert;
				status.pred_edge_number[edge.to] = edge_no;
				flow[edge.to] = edge_flow;
				vertices.push(edge.to);
			}

			++edge_no;
		}
	}
	return flow[end];
}

void UpdateGraph(Graph& graph, vertex_t start, vertex_t end, weight_t flow, const DfsStatus& status) {
	while (start != end) {
		vertex_t prev = status.pred[end];
		Edge& cur_edge = graph[prev][status.pred_edge_number[end]];
		Edge& back_edge = graph[end][cur_edge.back_index];

		cur_edge.flow += flow;
		back_edge.flow -= flow;

		end = prev;
	}
}

weight_t MaxFlow(Graph graph, vertex_t start, vertex_t end) {
	int64_t current_flow = 0;

	while (true) {
		DfsStatus status(graph.size());
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
		list_graph[from - 1].push_back({ from - 1, to - 1, weight });
		list_graph[to - 1].push_back({ to - 1, from - 1, 0 });

		list_graph[from - 1].back().back_index = list_graph[to - 1].size() - 1;
		list_graph[to - 1].back().back_index = list_graph[from - 1].size() - 1;
	}

	std::cout << MaxFlow(list_graph, start, end) << "\n";

	return 0;
}


