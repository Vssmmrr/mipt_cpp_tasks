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

bool Relax(const Edge& edge, std::vector<std::vector<int64_t>>& min_dist, std::vector<std::vector<vertex_t>>& prev, int64_t i) {
	if (min_dist[edge.to][i + 1] == kInfinity || min_dist[edge.to][i + 1] > min_dist[edge.from][i] + edge.weight) {
		min_dist[edge.to][i + 1] = min_dist[edge.from][i] + edge.weight;
		prev[edge.to][i + 1] = edge.from;
		return true; // did relax
	}
	return false; // didn't relax
}

int64_t GetShortestPath(const Graph& graph, vertex_t start, vertex_t end, int64_t num_nights) {
	uint64_t graph_size = graph.size();
	std::vector<std::vector<bool>> visited(graph_size, std::vector<bool>(num_nights + 1, false));
	std::vector<std::vector<vertex_t>> previous(graph_size, std::vector<vertex_t>(num_nights + 1, -1));
	std::vector<std::vector<int64_t>> distances(graph_size, std::vector<int64_t>(num_nights + 1, kInfinity));

	distances[start][0] = 0;
	std::vector<std::priority_queue<std::pair<int64_t, vertex_t>, std::vector< std::pair<int64_t, vertex_t>>, std::greater<std::pair<int64_t, vertex_t>>>> distances_heap(num_nights + 1);
	distances_heap[0].push({ distances[start][0], start });

	for (int64_t i = 0; i < num_nights; ++i) {
		while (!distances_heap[i].empty()) {
			auto current_vertex = distances_heap[i].top().second;
			auto cur_vert_dist = distances_heap[i].top().first;
			distances_heap[i].pop();

			if (!visited[current_vertex][i + 1]) {
				for (const auto& edge : graph[current_vertex]) {
					if (Relax(edge, distances, previous, i)) {
						distances_heap[i + 1].push({ distances[edge.to][i + 1], edge.to });
					}
				}
				visited[current_vertex][i + 1] = true;
			}
		}
	}

	int64_t distance = distances[end][num_nights];
	for (int64_t i = 0; i <= num_nights; ++i) {
		distance = std::min(distance, distances[end][i]);
	}

	return distance == kInfinity ? -1 : distance;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int64_t num_cities, num_flights, num_nights, start, end;
	std::cin >> num_cities >> num_flights >> num_nights >> start >> end;

	Graph graph(num_cities);
	for (int64_t i = 0; i < num_flights; ++i) {
		vertex_t from, to;
		int64_t weight;
		std::cin >> from >> to >> weight;
		graph[from - 1].push_back({ from - 1, to - 1, weight });
	}

	std::cout << GetShortestPath(graph, start - 1, end - 1, num_nights) << '\n';


	return 0;
}
