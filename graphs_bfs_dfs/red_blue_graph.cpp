#include <vector>
#include <unordered_set>
#include <iostream>
#include <cstring>
#include <utility>

//oriented
class Graph {
public:
    typedef uint32_t vertex_t;

    virtual vertex_t GetNumVertices() const = 0;
    virtual vertex_t GetNumEdges() const = 0;

    virtual void AddEdge(vertex_t from, vertex_t to) = 0;

    virtual std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const = 0;

    virtual void Transpose() = 0;
};

class AdjacencyListsGraph : public Graph {
public:
    explicit AdjacencyListsGraph(size_t num_vertices);
    AdjacencyListsGraph(size_t num_vertices, const std::vector<std::pair<vertex_t, vertex_t>>& edges_list);

    vertex_t GetNumVertices() const override;
    vertex_t GetNumEdges() const override;

    void AddEdge(vertex_t from, vertex_t to) override;

    std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const override;

    void Transpose() override;

private:
    std::vector<std::vector<vertex_t>> adjacency_lists_;
    size_t num_vertices_ = 0;
    size_t num_edges_ = 0;
};

enum Status {undiscovered, discovered, processed};


bool DfsVisitCycle(const AdjacencyListsGraph& graph, std::vector<Status>& visited,
                   Graph::vertex_t vertex) {
    visited[vertex] = discovered;

    std::vector<Graph::vertex_t> neigbours = graph.GetAdjacentVertices(vertex);
    for (auto& elem : neigbours) {
        if (visited[elem] == undiscovered) {
            if (DfsVisitCycle(graph, visited, elem)) {
                return true;
            }
        } else if (visited[elem] == discovered) {
            return true;
        }
    }

    visited[vertex] = processed;
    return false;
}

bool IsOneColor(const AdjacencyListsGraph& graph) {
    std::vector<Status> visited(graph.GetNumVertices(), undiscovered);
    std::vector<size_t> pred(graph.GetNumVertices(), -1);

    for (size_t i = 0; i < graph.GetNumVertices(); ++i) {
        if (visited[i] == undiscovered) {
            if (DfsVisitCycle(graph, visited, i)) {
                return false;
            }
        }
    }

    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    size_t num_vertex;
    std::cin >> num_vertex;

    AdjacencyListsGraph list_graph(num_vertex);

    for (size_t i = 0; i < num_vertex - 1; ++i) {
        for (size_t j = i + 1; j < num_vertex; ++j) {
            char color;
            std::cin >> color;
            if (color == 'R') {
                list_graph.AddEdge(j, i);
            } else if (color == 'B') {
                list_graph.AddEdge(i, j);
            }
        }
    }

    if (IsOneColor(list_graph)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}


AdjacencyListsGraph::AdjacencyListsGraph(size_t num_vertices) : num_vertices_(num_vertices) {
    adjacency_lists_.resize(num_vertices);
}
AdjacencyListsGraph::AdjacencyListsGraph(size_t num_vertices, const std::vector<std::pair<vertex_t, vertex_t>>& edges_list)
        : num_vertices_(num_vertices) {
    adjacency_lists_.resize(num_vertices);
    for (const auto& elem : edges_list) {
        AddEdge(elem.first, elem.second);
    }
}

Graph::vertex_t AdjacencyListsGraph::GetNumVertices() const {
    return num_vertices_;
}

Graph::vertex_t AdjacencyListsGraph::GetNumEdges() const {
    return num_edges_;
}


void AdjacencyListsGraph::AddEdge(vertex_t from, vertex_t to) {
    adjacency_lists_[from].push_back(to);
    ++num_edges_;
}


std::vector<Graph::vertex_t> AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
    std::vector<vertex_t> adjancent_vertices;
    for (const auto& elem : adjacency_lists_[vertex]) {
        adjancent_vertices.push_back(elem);
    }
    return adjancent_vertices;
}

void AdjacencyListsGraph::Transpose() {
    std::vector<std::vector<vertex_t>> new_adj_list(num_vertices_);
    for (auto first_it = adjacency_lists_.begin(); first_it != adjacency_lists_.end(); ++first_it) {
        for (auto second_it = (*first_it).begin(); second_it != (*first_it).end(); ++second_it) {
            new_adj_list[(*second_it)].push_back(first_it - adjacency_lists_.begin());
        }
    }
    adjacency_lists_ = new_adj_list;
}


