#include <iostream>
#include <vector>
#include <unordered_set>

typedef size_t vertex_t;

struct Edge {
    vertex_t from_;
    vertex_t to_;

    Edge(vertex_t from, vertex_t to) : from_(from), to_(to) {
    }
};

//oriented
class Graph {
public:


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
    explicit AdjacencyListsGraph(vertex_t num_vertices);
    AdjacencyListsGraph(vertex_t num_vertices, const std::vector<Edge>& edges_list);

    vertex_t GetNumVertices() const override;
    size_t GetNumEdges() const override;

    bool HasEdge(vertex_t from, vertex_t to) const override;
    void AddEdge(vertex_t from, vertex_t to) override;
    void DeleteEdge(vertex_t from, vertex_t to) override;

    std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const override;

    void Transpose() override;

private:
    std::vector<std::unordered_multiset<vertex_t>> adjacency_lists_;
    size_t num_edges_ = 0;
};

enum Status {undiscovered, discovered, processed};


void DfsVisit(const AdjacencyListsGraph& graph, std::vector<Status>& visited,
              vertex_t vertex, std::vector<vertex_t>& answer) {
    visited[vertex] = discovered;
    std::vector<vertex_t> neigbours = graph.GetAdjacentVertices(vertex);
    for (const auto& elem : neigbours) {
        if (visited[elem] == undiscovered) {
            DfsVisit(graph, visited, elem, answer);
        }
    }

    visited[vertex] = processed;
    answer.push_back(vertex);
}


std::vector<std::vector<vertex_t>> FindChats(const AdjacencyListsGraph& graph) {
    std::vector<std::vector<vertex_t>> chats;
    std::vector<Status> visited(graph.GetNumVertices(), undiscovered);
    for (vertex_t vertex = 0; vertex < graph.GetNumVertices(); ++vertex) {
        if (visited[vertex] == undiscovered) {
            chats.emplace_back();
            DfsVisit(graph, visited, vertex, chats.back());
        }
    }
    return chats;
}


int main() {
    size_t num_students, num_pairs;
    std::cin >> num_students >> num_pairs;
    std::vector<Edge> edges;
    for (size_t i = 0; i < num_pairs; ++i) {
        vertex_t from, to;
        std::cin >> from >> to;
        edges.push_back({from - 1, to - 1});
        edges.push_back({to - 1, from - 1});
    }

    AdjacencyListsGraph graph(num_students, edges);

    std::vector<std::vector<vertex_t>> chats = FindChats(graph);

    std::cout << chats.size() << '\n';
    for (size_t chat_num = 0; chat_num < chats.size(); ++chat_num) {
        std::cout << chats[chat_num].size() << '\n';
        for (const auto& student : chats[chat_num]) {
            std::cout << student + 1 << " ";
        }
        std::cout << '\n';
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

bool AdjacencyListsGraph::HasEdge(vertex_t from, vertex_t to) const {
    return adjacency_lists_[from].find(to) != adjacency_lists_[from].end();
}
void AdjacencyListsGraph::AddEdge(vertex_t from, vertex_t to) {
    adjacency_lists_[from].insert(to);
    ++num_edges_;
}

//if there are multiple edges, it removes only one
void AdjacencyListsGraph::DeleteEdge(vertex_t from, vertex_t to) {
    if (HasEdge(from, to)) {
        adjacency_lists_[from].erase(to);
        --num_edges_;
    }
}

std::vector<vertex_t> AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
    return {adjacency_lists_[vertex].begin(), adjacency_lists_[vertex].end()};
}

void AdjacencyListsGraph::Transpose() {
    std::vector<std::unordered_multiset<vertex_t>> new_adj_list(adjacency_lists_.size());
    for (auto first_it = adjacency_lists_.begin(); first_it != adjacency_lists_.end(); ++first_it) {
        for (auto vert : *first_it) {
            new_adj_list[vert].insert(first_it - adjacency_lists_.begin());
        }
    }
    adjacency_lists_ = std::move(new_adj_list);
}

