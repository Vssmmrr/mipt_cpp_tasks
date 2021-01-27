#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

typedef size_t vertex_t;

const int kInfinity = 30000;

struct Edge {
	vertex_t from;
	vertex_t to;
	int weight;
};

typedef std::vector<std::vector<Edge>> Graph;

bool Relax(const Edge edge, std::vector<int>& min_dist, std::vector<vertex_t>& prev) {
	if (min_dist[edge.from] != kInfinity && min_dist[edge.to] > min_dist[edge.from] + edge.weight) {
		min_dist[edge.to] = min_dist[edge.from] + edge.weight;
		prev[edge.to] = edge.from;
		return true; // did relax
	}
	return false; // didn't relax
}

std::vector<int> GetShortestPath(const Graph& graph, vertex_t start) {
	std::vector<int> min_distance(graph.size(), kInfinity);
	std::vector<vertex_t> previous(graph.size(), -1);
	min_distance[start] = 0;
	std::queue<vertex_t> updated_dist_vertexes;
	std::vector<bool> in_queue(graph.size(), false);
	updated_dist_vertexes.push(start);
	in_queue[start] = true;

	while (!updated_dist_vertexes.empty()) {
		auto vert = updated_dist_vertexes.front();
		updated_dist_vertexes.pop();
		in_queue[vert] = false;
		for (const auto& edge : graph[vert]) {
			if (Relax(edge, min_distance, previous) && !in_queue[edge.to]) {
				updated_dist_vertexes.push(edge.to);
				in_queue[edge.to] = true;
			}
		}
	}

	return min_distance;
}

int main() {
	size_t num_vertices, num_edges;
	std::cin >> num_vertices >> num_edges;

	Graph graph(num_vertices);
	for (size_t i = 0; i < num_edges; ++i) {
		vertex_t from, to;
		int weight;
		std::cin >> from >> to >> weight;
		graph[from - 1].push_back({ from - 1, to - 1, weight });
	}

	auto distances = GetShortestPath(graph, 0);
	for (const auto& elem : distances) {
		std::cout << elem << ' ';
	}

	system("pause");
	return 0;
}
