#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

const int kUnknown = -1;

typedef size_t vertex_t;

//not oriented
class Graph {
public:

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

struct DfsStatus {
    int time;
    std::vector<int> t_in;
    std::vector<int> t_up;
    std::vector<std::pair<vertex_t, vertex_t>> answer;
    std::vector<int> cnt_edges_to_prev;
};

void Dfs(const AdjacencyListsGraph& graph, vertex_t vertex, vertex_t previous, DfsStatus& status) {
    ++status.time;
    status.t_in[vertex] = status.time;
    status.t_up[vertex] = status.time;
    auto neighbours = graph.GetAdjacentVertices(vertex);
    for (const auto& elem : neighbours) {
        if (elem == previous) {
            ++status.cnt_edges_to_prev[vertex];
            continue;
        } else if (status.t_in[elem] != kUnknown) {
            status.t_up[vertex] = std::min(status.t_up[vertex], status.t_in[elem]);
        }
        if (status.t_in[elem] == kUnknown) {
            Dfs(graph, elem, vertex, status);
            status.t_up[vertex] = std::min(status.t_up[vertex], status.t_up[elem]);
            if (status.t_in[vertex] < status.t_up[elem] && status.cnt_edges_to_prev[elem] == 1) {
                status.answer.push_back({vertex, elem});
            }
        }
    }
}

std::vector<std::pair<vertex_t, vertex_t>> GetBridges(const AdjacencyListsGraph& graph) {
    const auto size = graph.GetNumVertices();
    DfsStatus status;
    status.time = 0;
    status.t_up.resize(size, kUnknown);
    status.t_in.resize(size, kUnknown);
    status.cnt_edges_to_prev.resize(size, 0);

    for (size_t root = 0; root < size; ++root) {
        if (status.t_in[root] == kUnknown) {
            Dfs(graph, root, kUnknown, status);
        }
    }

    return status.answer;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertex, num_edges;
    std::cin >> num_vertex >> num_edges;

    std::map<std::pair<vertex_t, vertex_t>, size_t> order_of_edges;
    AdjacencyListsGraph list_graph(num_vertex);
    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        std::cin >> from >> to;
        list_graph.AddEdge(from - 1, to - 1);
        order_of_edges[{from - 1, to - 1}] = i;
        order_of_edges[{to - 1, from - 1}] = i;
    }

    auto bridges = GetBridges(list_graph);
    std::vector<size_t> result;
    for (size_t i = 0; i < bridges.size(); ++i) {
        auto it = order_of_edges.find(bridges[i]);
        if (it != order_of_edges.end()) {
            result.push_back(it->second);
        }
    }
    std::sort(result.begin(), result.end());
    size_t num_bridges = result.size();
    std::cout << num_bridges << '\n';
    for (size_t i = 0; i < num_bridges; ++i) {
        std::cout << result[i] + 1 << '\n';
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

vertex_t AdjacencyListsGraph::GetNumVertices() const {
    return num_vertices_;
}

vertex_t AdjacencyListsGraph::GetNumEdges() const {
    return num_edges_;
}


void AdjacencyListsGraph::AddEdge(vertex_t from, vertex_t to) {
    adjacency_lists_[from].push_back(to);
    adjacency_lists_[to].push_back(from);
    ++num_edges_;
}


std::vector<vertex_t> AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
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

