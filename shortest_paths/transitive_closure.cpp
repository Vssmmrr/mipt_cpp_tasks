#include <algorithm>
#include <iostream>
#include <vector>

const int kNoEdge = 0;
const int kInfinity = 100000;
typedef size_t vertex_t;
typedef std::vector<std::vector<int>> Distance_matrix;

Distance_matrix GetAllDistances(Distance_matrix distances) {
	for (size_t k = 0; k < distances.size(); ++k) {
		for (size_t x = 0; x < distances.size(); ++x) {
			for (size_t y = 0; y < distances.size(); ++y) {
				if (distances[k][y] + distances[x][k] < distances[x][y]) {
					distances[x][y] = distances[x][k] + distances[k][y];
				}
			}
		}
	}

	return distances;
}

int main() {
	size_t num_vertices;
	std::cin >> num_vertices;

	Distance_matrix graph(num_vertices);
	for (size_t i = 0; i < num_vertices; ++i) {
		for (size_t j = 0; j < num_vertices; ++j) {
			int value;
			std::cin >> value;
			if (value == kNoEdge) {
				value = kInfinity;
			}
			graph[i].push_back(value);
		}
	}

	auto distances = GetAllDistances(graph);
	for (size_t i = 0; i < num_vertices; ++i) {
		for (size_t j = 0; j < num_vertices; ++j) {
			if (distances[i][j] < kInfinity) {
				distances[i][j] = 1;
			} else {
				distances[i][j] = 0;
			}
			std::cout << distances[i][j] << ' ';
		}
		std::cout << '\n';
	}

	return 0;
}
