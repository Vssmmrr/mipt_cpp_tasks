#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

const int kOneSet = 1;

class Dsu {
public:
    typedef int elem_t;

    explicit Dsu(size_t n_sets);
    elem_t MakeSet();
    elem_t FindSet(elem_t elem) const;
    void Union(elem_t first, elem_t second);
    size_t CountDifferentSets() const;

private:
    size_t count_different_sets_;
    mutable std::vector<elem_t> pred_;
    std::vector<size_t> ranks_;
};

struct Edge {
    size_t from_;
    size_t to_;
    int weight_;

    Edge(size_t from, size_t to, int weight) : from_(from), to_(to), weight_(weight) {
    }
};

bool operator<(const Edge& lhs, const Edge& rhs) {
    return lhs.weight_ < rhs.weight_;
}

typedef std::vector<Edge> Graph;

std::vector<Edge> Kruscal(Graph graph, size_t num_vertices) {
    std::vector<Edge> min_spanning_tree;
    Dsu connected_components(num_vertices);
    std::sort(graph.begin(), graph.end());

    for (const auto& edge : graph) {
        if (connected_components.FindSet(edge.from_) != connected_components.FindSet(edge.to_)) {
            min_spanning_tree.push_back(edge);
            connected_components.Union(edge.from_, edge.to_);
        }
    }

    return min_spanning_tree;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertices, num_edges;
    std::cin >> num_vertices >> num_edges;

    Graph graph;
    for (size_t i = 0; i < num_edges; ++i) {
        size_t from, to;
        int weight;
        std::cin >> from >> to >> weight;
        graph.push_back({from - 1, to - 1, weight});
    }

    auto min_spanning_tree = Kruscal(graph, num_vertices);
    int min_weight = 0;
    for (const auto& elem : min_spanning_tree) {
        min_weight += elem.weight_;
    }
    std::cout << min_weight << '\n';
    return 0;
}

Dsu::Dsu(size_t n_sets) : count_different_sets_(n_sets), pred_(n_sets), ranks_(n_sets, 0) {
    std::iota(pred_.begin(), pred_.end(), 0);
}

Dsu::elem_t Dsu::MakeSet() {
    pred_.push_back(pred_.size());
    ranks_.push_back(0);
    ++count_different_sets_;
    return pred_.size() - 1;
}

Dsu::elem_t Dsu::FindSet(Dsu::elem_t elem) const {
    if (pred_[elem] == elem) {
        return elem;
    }
    pred_[elem] = FindSet(pred_[elem]);
    return pred_[elem];
}

void Dsu::Union(Dsu::elem_t first, Dsu::elem_t second) {
    first = FindSet(first);
    second = FindSet(second);
    if (first == second) {
        return;
    }
    --count_different_sets_;
    if (ranks_[first] < ranks_[second]) {
        pred_[first] = second;
    } else if (ranks_[first] > ranks_[second]) {
        pred_[second] = first;
    } else {
        ++ranks_[first];
        pred_[second] = first;
    }
}

size_t Dsu::CountDifferentSets() const {
    return count_different_sets_;
}

