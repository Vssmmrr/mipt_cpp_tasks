#include <vector>
#include <unordered_set>
#include <iostream>
#include <utility>

//oriented
class Graph {
public:
    typedef size_t vertex_t;

    virtual vertex_t GetNumVertices() const = 0;
    virtual size_t GetNumEdges() const = 0;

    virtual bool HasEdge(vertex_t from, vertex_t to) const = 0;
    virtual void AddEdge(vertex_t from, vertex_t to) = 0;
    virtual void DeleteEdge(vertex_t from, vertex_t to) = 0;

    virtual std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const = 0;

    virtual void Transpose() = 0;
};

class AdjacencyListsGraph : public Graph {
public:
    explicit AdjacencyListsGraph(size_t num_vertices);
    AdjacencyListsGraph(size_t num_vertices, const std::vector<std::pair<vertex_t, vertex_t>>& edges_list);

    size_t GetNumVertices() const override;
    size_t GetNumEdges() const override;

    bool HasEdge(vertex_t from, vertex_t to) const override;
    void AddEdge(vertex_t from, vertex_t to) override;
    void DeleteEdge(vertex_t from, vertex_t to) override;

    std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const override;

    void Transpose() override;

private:
    std::vector<std::unordered_multiset<vertex_t>> adjacency_lists_;
    size_t num_vertices_ = 0;
    size_t num_edges_ = 0;
};

enum Status {undiscovered, discovered, processed};


bool DfsVisitCycle(const AdjacencyListsGraph& graph, std::vector<Status>& visited,
               Graph::vertex_t vertex, Graph::vertex_t& start_loop, std::vector<Graph::vertex_t>& pred) {
    visited[vertex] = discovered;

    std::vector<Graph::vertex_t> neigbours = graph.GetAdjacentVertices(vertex);
    for (auto& elem : neigbours) {
        if (visited[elem] == undiscovered) {
            if (DfsVisitCycle(graph, visited, elem, start_loop, pred)) {
                pred[elem] = vertex;
                return true;
            }
        } else if (visited[elem] == discovered) {
            start_loop = vertex;
            return true;
        }
    }

    visited[vertex] = processed;
    return false;
}

std::vector<Graph::vertex_t> FindCyclePath(const AdjacencyListsGraph& graph, std::vector<Graph::vertex_t>& pred,
                                  const Graph::vertex_t start_loop) {
    std::vector<Graph::vertex_t> cycle_path;
    Graph::vertex_t current_vertex = start_loop;
    while (!graph.HasEdge(start_loop, current_vertex)) {
        cycle_path.push_back(current_vertex);
        current_vertex = pred[current_vertex];
    }
    cycle_path.push_back(current_vertex);
    return {cycle_path.rbegin(), cycle_path.rend()};
}

std::vector<size_t> Cycle(const AdjacencyListsGraph& graph) {
    std::vector<Status> visited(graph.GetNumVertices(), undiscovered);
    std::vector<size_t> pred(graph.GetNumVertices(), -1);
    Graph::vertex_t start_loop = -1;

    for (size_t i = 0; i < graph.GetNumVertices(); ++i) {
        if (visited[i] == undiscovered) {
            if (DfsVisitCycle(graph, visited, i, start_loop, pred)) {
                break;
            }
        }
    }

    if (start_loop == -1) {
        return {};
    } else {
       return FindCyclePath(graph, pred, start_loop);
    }
}

int main() {
    size_t num_vertex, num_edges;
    std::cin >> num_vertex >> num_edges;
    std::vector<std::pair<size_t, size_t>> edges_list;
    for (size_t i = 0; i < num_edges; ++i) {
        int from;
        int to;
        std::cin >> from >> to;
        edges_list.push_back({ from - 1, to - 1 });
    }

    AdjacencyListsGraph list_graph(num_vertex, edges_list);

    std::vector<Graph::vertex_t> cycle = Cycle(list_graph);
    if (cycle.empty()) {
        std::cout << "NO\n";
    } else {
        std::cout << "YES\n";
        for (const auto& elem : cycle) {
            std::cout << elem + 1 << ' ';
        }
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

size_t AdjacencyListsGraph::GetNumVertices() const {
    return num_vertices_;
}

size_t AdjacencyListsGraph::GetNumEdges() const {
    return num_edges_;
}

bool AdjacencyListsGraph::HasEdge(vertex_t from, vertex_t to) const {
    return (adjacency_lists_[from].count(to) != 0);
}
void AdjacencyListsGraph::AddEdge(vertex_t from, vertex_t to) {
    adjacency_lists_[from].insert(to);
    ++num_edges_;
}

//if there are multiple edges, it removes only one
void AdjacencyListsGraph::DeleteEdge(vertex_t from, vertex_t to) {
    adjacency_lists_[from].erase(to);
    --num_edges_;
}

std::vector<Graph::vertex_t> AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
    std::vector<vertex_t> adjancent_vertices;
    for (const auto& elem : adjacency_lists_[vertex]) {
        adjancent_vertices.push_back(elem);
    }
    return adjancent_vertices;
}

void AdjacencyListsGraph::Transpose() {
    std::vector<std::unordered_multiset<vertex_t>> new_adj_list(num_vertices_);
    for (auto first_it = adjacency_lists_.begin(); first_it != adjacency_lists_.end(); ++first_it) {
        for (auto second_it = (*first_it).begin(); second_it != (*first_it).end(); ++second_it) {
            new_adj_list[(*second_it)].insert(first_it - adjacency_lists_.begin());
        }
    }
    adjacency_lists_ = new_adj_list;
}

