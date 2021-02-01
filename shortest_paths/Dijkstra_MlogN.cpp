#include <iostream>
#include <queue>
#include <vector>

typedef int64_t vertex_t;

struct Edge {
	vertex_t from;
	vertex_t to;
	int64_t weight;
};

typedef std::vector<std::vector<Edge>> Graph;
const int64_t kInfinity = 2009000999;

bool Relax(const Edge& edge, std::vector<int64_t>& min_dist, std::vector<vertex_t>& prev) {
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
				if (Relax(edge, distances, previous)) {
					distances_heap.push({ distances[edge.to], edge.to });
				}
			}
			visited[current_vertex] = true;
		}
	}
	return distances;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int64_t NUM;
	std::cin >> NUM;
	for (int64_t i = 0; i < NUM; ++i) {
		int64_t num_vertices, num_edges;
		std::cin >> num_vertices >> num_edges;

		Graph graph(num_vertices);
		for (int64_t j = 0; j < num_edges; ++j) {
			vertex_t from, to;
			int64_t weight;
			std::cin >> from >> to >> weight;
			graph[from].push_back({ from, to, weight });
			graph[to].push_back({ to, from, weight });
		}
		
		int64_t start;
		std::cin >> start;
		auto distances = GetDistances(graph, start);
		for (const auto& elem : distances) {
			std::cout << elem << " ";
		}
		std::cout << '\n';
	}
	//system("pause");
	return 0;
}
