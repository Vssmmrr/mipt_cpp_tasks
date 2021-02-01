#include <vector>
#include <iostream>

typedef size_t vertex_t;

const int kInfinity = 100000;

struct Edge {
	vertex_t from;
	vertex_t to;
	int weight;
};

typedef std::vector<Edge> Graph;

bool Relax(const Edge edge, std::vector<int>& min_dist) {
	if (min_dist[edge.from] != kInfinity && min_dist[edge.to] > min_dist[edge.from] + edge.weight) {
		min_dist[edge.to] = min_dist[edge.from] + edge.weight;
		return true; // did relax
	}
	return false; // didn't relax
}

int GetShortestPathCost(const Graph& graph, size_t num_vertices, vertex_t start, vertex_t end) {
	std::vector<int> min_distance(num_vertices, kInfinity);
	min_distance[start] = 0;

	for (size_t i = 0; i < num_vertices - 1; ++i) {
		for (const auto& neighbour : graph) {
			Relax(neighbour, min_distance);
		}
	}

	return min_distance[end];
}

int main() {
	size_t num_cities;
	std::cin >> num_cities;
	std::vector<int> cities_cost(num_cities);
	for (size_t i = 0; i < num_cities; ++i) {
		std::cin >> cities_cost[i];
	}

	size_t num_roads;
	std::cin >> num_roads;
	std::vector<Edge> roads;
	roads.reserve(2 * num_roads);
	for (size_t i = 0; i < num_roads; ++i) {
		size_t from;
		size_t to;

		std::cin >> from >> to;
		--from;
		--to;
		roads.push_back({ from, to, cities_cost[from] });
		roads.push_back({ to, from, cities_cost[to] });
	}
	auto cost = GetShortestPathCost(roads, num_cities, 0, num_cities - 1);
	if (cost == kInfinity) {
		std::cout << "-1\n";
	} else {
		std::cout << cost << "\n";
	}
	system("pause");
}

