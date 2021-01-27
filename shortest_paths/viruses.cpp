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

std::vector<int64_t> GetDistances(const Graph& graph, std::vector<vertex_t> starts) {
	uint64_t graph_size = graph.size();
	std::vector<bool> visited(graph_size, false);
	std::vector<vertex_t> previous(graph_size, -1);
	std::vector<int64_t> distances(graph_size, kInfinity);
	std::priority_queue<std::pair<int64_t, vertex_t>, std::vector< std::pair<int64_t, vertex_t>>, std::greater<std::pair<int64_t, vertex_t>>> distances_heap;

	for (auto elem : starts) {
		distances[elem] = 0;
		distances_heap.push({ distances[elem], elem });
	}
	
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

	int64_t num_countries, num_roads, num_infected;
	std::cin >> num_countries >> num_roads >> num_infected;
	std::vector<int64_t> infected_countries;
	for (int64_t i = 0; i < num_infected; ++i) {
		int64_t infected;
		std::cin >> infected;
		infected_countries.push_back(infected - 1);
	}

	Graph countries(num_countries);
	for (int64_t i = 0; i < num_roads; ++i) {
		int64_t from, to, distance;
		std::cin >> from >> to >> distance;
		countries[from - 1].push_back({ from - 1, to - 1, distance });
		countries[to - 1].push_back({ to - 1, from - 1, distance });
	}

	int64_t start, spaceport;
	std::cin >> start >> spaceport;
	--start;
	--spaceport;
	
	auto infected_roads = GetDistances(countries, infected_countries);
	auto people_way = GetDistances(countries, { start });

	if (infected_roads[spaceport] <= people_way[spaceport]) {
		std::cout << "-1\n";
	}
	else {
		std::cout << people_way[spaceport] << "\n";
	}
	
	system("pause");
	return 0;
}
