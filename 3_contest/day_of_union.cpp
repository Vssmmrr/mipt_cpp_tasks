#include <iostream>
#include <cmath>
#include <queue>
#include <vector>
#include <utility>
#include <iomanip>

typedef size_t vertex_t;
typedef std::pair<int, int> coordinates;
const double INF = 1000000000;

struct Edge {
    size_t from_;
    size_t to_;
    double weight_;

    Edge() = default;
    Edge(size_t from, size_t to, double weight) : from_(from), to_(to), weight_(weight) {
    }
};

bool operator>(const Edge& lhs, const Edge& rhs) {
    return lhs.weight_ > rhs.weight_;
}

class Graph {
private:
    std::vector<coordinates> vertices_;

public:
    Graph(const std::vector<coordinates>& vertices) : vertices_(vertices) {
    }

    std::vector<std::pair<vertex_t, double>> GetAdjacentVertices(size_t vert) const {
        std::vector<std::pair<vertex_t, double>> result;

        for (size_t i = 0; i < vertices_.size(); ++i) {
            if (i == vert) {
                continue;
            }
            double weight = sqrt((vertices_[i].first - vertices_[vert].first) * (vertices_[i].first - vertices_[vert].first) +
                                 (vertices_[i].second - vertices_[vert].second) * (vertices_[i].second - vertices_[vert].second));
            result.push_back({i, weight});
        }
        return result;
    }

    size_t GetNumVertices() const {
        return vertices_.size();
    }
};

double Prim(const Graph& graph) {
    size_t num_vertices = graph.GetNumVertices();
    std::vector<double> d(num_vertices, INF);
    std::vector<bool> used(num_vertices, false);
    std::vector<Edge> answer;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> q;

    vertex_t current = 0;
    used[0] = true;

    for (const auto [vert, weight] : graph.GetAdjacentVertices(current)) {
        q.push({current, vert, weight});
    }
    while (answer.size() < (num_vertices - 1)) {
        Edge current_edge = q.top();
        q.pop();
        if (used[current_edge.to_]) {
            continue;
        }
        used[current_edge.to_] = true;
        answer.push_back(current_edge);
        current = current_edge.to_;
        for (const auto elem : graph.GetAdjacentVertices(current)) {
            if (!used[elem.first] && d[elem.first] > elem.second) {
                d[elem.first] = elem.second;
                q.push({current, elem.first, elem.second});
            }
        }
    }

    double min_weight_span_tree = 0;
    for (const auto& elem : answer) {
        min_weight_span_tree += elem.weight_;
    }
    return min_weight_span_tree;
}

int main() {
    size_t num_cities;
    std::cin >> num_cities;

    std::vector<coordinates> cities;
    for (size_t i = 0; i < num_cities; ++i) {
        coordinates city;
        std::cin >> city.first >> city.second;
        cities.push_back(city);
    }

    Graph graph(cities);

    std::cout << std::fixed << std::setprecision(6) << Prim(graph) << '\n';

    return 0;
}

