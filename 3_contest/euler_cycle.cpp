#include <iostream>
#include <cmath>
#include <queue>
#include <vector>
#include <utility>
#include <unordered_map>

typedef size_t vertex_t;

struct Edge {
    vertex_t from_;
    vertex_t to_;

    Edge(vertex_t from, vertex_t to) : from_(from), to_(to) {
    }
};

bool operator<(const Edge& lhs, const Edge& rhs) {
    return lhs.from_ < rhs.from_;
}

class AdjacencyListsGraph {
public:
    explicit AdjacencyListsGraph(vertex_t num_vertices);
    AdjacencyListsGraph(vertex_t num_vertices, const std::vector<Edge>& edges_list);

    vertex_t GetNumVertices() const;
    size_t GetNumEdges() const;

    bool HasEdge(vertex_t from, vertex_t to) const;
    void AddEdge(vertex_t from, vertex_t to);
    void DeleteEdge(vertex_t from, vertex_t to);

    const std::unordered_map<vertex_t, size_t>& GetAdjacentVertices(vertex_t vertex) const;

private:
    std::vector<std::unordered_map<vertex_t, size_t>> adjacency_lists_;
    size_t num_edges_ = 0;

};

void EulerCycle(AdjacencyListsGraph& graph, vertex_t vertex,
                std::vector<vertex_t>& answer) {
    const auto& neigbours = graph.GetAdjacentVertices(vertex);
    for (auto iter = neigbours.begin(); iter != neigbours.end(); ++iter) {
        while (iter->second > 0) {
            graph.DeleteEdge(vertex, iter->first);
            EulerCycle(graph, iter->first, answer);
        }
    }
    answer.push_back(vertex);
}

std::vector<vertex_t> Cycle(AdjacencyListsGraph graph) {
    size_t num_edges = graph.GetNumEdges();
    size_t num_vertices = graph.GetNumVertices();
    std::vector<vertex_t> answer;

    for (vertex_t vertex = 0; vertex < num_vertices; ++vertex) {
        EulerCycle(graph, vertex, answer);
        if (answer.size() != 1) {
            break;
        } else {
            answer.clear();
        }
    }

    if (answer.size() != num_edges + 1) {
        return {};
    }

    return {answer.rbegin(), answer.rend()};
}


int main() {
    size_t num_ways, num_squares;
    std::cin >> num_ways >> num_squares;

    AdjacencyListsGraph graph(num_squares);
    for (size_t i = 0; i < num_ways; ++i) {
        size_t num_elem;
        std::cin >> num_elem;
        vertex_t first, second;
        std::cin >> first;
        for (size_t j = 0; j < num_elem; ++j) {
            std::cin >> second;
            graph.AddEdge(first - 1, second - 1);
            first = second;
        }
    }

    auto answer = Cycle(graph);
    if (answer.empty()) {
        std::cout << "0\n";
    } else {
        std::cout << answer.size() << ' ';
        for (const auto &elem: answer) {
            std::cout << elem + 1 << ' ';
        }
    }
    return 0;
}

AdjacencyListsGraph::AdjacencyListsGraph(vertex_t num_vertices)
        : adjacency_lists_(num_vertices) {
}

AdjacencyListsGraph::AdjacencyListsGraph(vertex_t num_vertices, const std::vector<Edge>& edges_list)
        : AdjacencyListsGraph(num_vertices) {
    for (const auto& elem : edges_list) {
        AddEdge(elem.from_, elem.to_);
    }
}

vertex_t AdjacencyListsGraph::GetNumVertices() const {
    return adjacency_lists_.size();
}

size_t AdjacencyListsGraph::GetNumEdges() const {
    return num_edges_;
}


void AdjacencyListsGraph::AddEdge(vertex_t from, vertex_t to) {
    ++adjacency_lists_[from][to];
    ++num_edges_;
}

void AdjacencyListsGraph::DeleteEdge(vertex_t from, vertex_t to) {
    auto iter = adjacency_lists_[from].find(to);
    if (iter != adjacency_lists_[from].end() && iter->second > 0) {
        --iter->second;
//        if (iter->second == 0) {
//            adjacency_lists_[from].erase(iter);
//        }
        --num_edges_;
    }
}

const std::unordered_map<vertex_t, size_t>& AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
    return adjacency_lists_[vertex];
}

