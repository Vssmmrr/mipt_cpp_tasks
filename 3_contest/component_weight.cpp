#include <iostream>
#include <numeric>
#include <vector>

typedef size_t vertex_t;
const int kAddEdge = 1;

class DsuGraph {
public:
    DsuGraph(size_t num_vertices) :
        pred_(num_vertices), rank_(num_vertices, 0), weight_(num_vertices, 0) {
        std::iota(pred_.begin(), pred_.end(), 0);
    }

    vertex_t GetComponent(vertex_t vertex) const {
        if (pred_[vertex] == vertex) {
            return vertex;
        }
        pred_[vertex] = GetComponent(pred_[vertex]);
        return pred_[vertex];
    }

    int GetWeight(vertex_t vertex) const {
        return weight_[GetComponent(vertex)];
    }

    void AddEdge(vertex_t from, vertex_t to, int weight) {
        from = GetComponent(from);
        to = GetComponent(to);
        if (from == to) {
            weight_[from] += weight;
            return;
        }
        if (rank_[from] > rank_[to]) {
            pred_[to] = from;
            weight_[from] += weight + weight_[to];
        } else {
            if (rank_[from] == rank_[to]) {
                ++rank_[to];
            }
            pred_[from] = to;
            weight_[to] += weight + weight_[from];
        }
    }

private:
    mutable std::vector<vertex_t> pred_;
    mutable std::vector<int> rank_;
    std::vector<int> weight_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertices, num_operations;
    std::cin >> num_vertices >> num_operations;
    DsuGraph graph(num_vertices);
    for (size_t i = 0; i < num_operations; ++i) {
        int operation;
        std::cin >> operation;
        if (operation == kAddEdge) {
            vertex_t from, to;
            int weight;
            std::cin >> from >> to >> weight;
            graph.AddEdge(from - 1, to - 1, weight);
        } else {
            vertex_t vertex;
            std::cin >> vertex;
            std::cout << graph.GetWeight(vertex - 1) << "\n";
        }
    }
    return 0;
}

