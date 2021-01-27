#include <iostream>
#include <vector>

const long int kInfinity = 1000000000;

typedef int vertex_t;

struct Edge {
	vertex_t from;
	vertex_t to;
	int weight;
};

typedef std::vector<std::vector<Edge>> Graph;

vertex_t FindMinDistNotVisited(std::vector<bool>& visited, std::vector<int> distances) {
	const size_t num_vertices = visited.size();
	vertex_t vert = -1;
	int min_distance = kInfinity;
	for (int current_vertex = 0; current_vertex < num_vertices; ++current_vertex) {
		if ((!visited[current_vertex]) && distances[current_vertex] < min_distance) {
			min_distance = distances[current_vertex];
			vert = current_vertex;
		}
	}
	return vert;
}

bool Relax(const Edge& edge, std::vector<int>& min_dist, std::vector<vertex_t>& prev) {
	if (min_dist[edge.to] > min_dist[edge.from] + edge.weight) {
		min_dist[edge.to] = min_dist[edge.from] + edge.weight;
		prev[edge.to] = edge.from;
		return true; // did relax
	}
	return false; // didn't relax
}

std::vector<int> Dijkstra(const Graph& graph, vertex_t start, vertex_t end) {
	const size_t graph_size = graph.size();
	std::vector<bool> visited(graph_size, false);
	std::vector<vertex_t> previous(graph_size, -1);
	std::vector<int> distances(graph_size, kInfinity);

	distances[start] = 0;
	for (size_t i = 0; i < graph_size; ++i) {
		vertex_t vert = FindMinDistNotVisited(visited, distances);
		
		if (vert < 0) {
			break;
		}

		for (const auto& edge : graph[vert]) {
			if (!visited[edge.to]) {
				Relax(edge, distances, previous);
			}
		}

		visited[vert] = true;
	}

	return distances;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int num_vertices;
	vertex_t start, end;
	std::cin >> num_vertices >> start >> end;

	Graph graph(num_vertices); //adj_list
	for (int i = 0; i < num_vertices; ++i) {
		for (int j = 0; j < num_vertices; ++j) {
			int distance;
			std::cin >> distance;
			if (distance >= 0) {
				graph[i].push_back({ i, j, distance });
			}
		}
	}

	auto distances = Dijkstra(graph, start - 1, end - 1);
	if (distances[end - 1] == kInfinity) {
		std::cout << "-1\n";
	} else {
		std::cout << distances[end - 1] << '\n';
	}


	return 0;
}
