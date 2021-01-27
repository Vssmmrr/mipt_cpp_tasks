#include <algorithm>
#include <iostream>
#include <vector>

typedef int vertex_t;

const int kInfinity = 100000;

struct Edge {
	vertex_t from;
	vertex_t to;
	int weight;
};

typedef std::vector<Edge> Graph;

bool Relax(const Edge edge, std::vector<int>& min_dist, std::vector<vertex_t>& prev) {
	if ( min_dist[edge.to] > min_dist[edge.from] + edge.weight) {
		min_dist[edge.to] = min_dist[edge.from] + edge.weight;
		prev[edge.to] = edge.from;
		return true; // did relax
	}
	return false; // didn't relax
}

std::vector<vertex_t> FindNegativeCycle(const Graph& graph, size_t num_vertices) {
	std::vector<int> min_distance(num_vertices, kInfinity);
	std::vector<vertex_t> previous(num_vertices, -1);

	for (size_t i = 0; i < num_vertices - 1; ++i) {
		for (const auto& edge : graph) {
				Relax(edge, min_distance, previous);
		}
	}

	for (const auto& edge : graph) {
		if (Relax(edge, min_distance, previous)) { // has cycle
			std::vector<bool> visited(num_vertices, false);
			vertex_t vert = edge.to;
			while (!visited[vert]) {
				visited[vert] = true;
				vert = previous[vert];
			}
			std::vector<vertex_t> negative_cycle;

			negative_cycle.push_back(vert);
			vertex_t u = previous[vert];
			while (u != vert) {
				negative_cycle.push_back(u);
				u = previous[u];
			}
			negative_cycle.push_back(vert);

			return {negative_cycle.rbegin(), negative_cycle.rend()};
		}
	}

	return {};
}

int main() {
	size_t num_vertices;
	std::cin >> num_vertices;

	Graph graph;
	for (int i = 0; i < num_vertices; ++i) {
		for (int j = 0; j < num_vertices; ++j) {
			int weight;
			std::cin >> weight;
			if (weight != 100000) {
				graph.push_back({ i, j, weight });
			}
		}
	}

	auto cycle = FindNegativeCycle(graph, num_vertices);
	std::cout << (!cycle.empty() ? "YES\n" : "NO\n");
	if (!cycle.empty()) {
		std::cout << cycle.size() << '\n';
		for (const auto& elem : cycle) {
			std::cout << elem + 1 << " ";
		}
	}

	//system("pause");
	return 0;
}
