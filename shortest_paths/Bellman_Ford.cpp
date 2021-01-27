#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
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

	for (size_t i = 0; i < graph.size() - 1; ++i) {
		for (const auto& vertex : graph) {
			for (const auto& neighbour : vertex) {
				Relax(neighbour, min_distance, previous);
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
