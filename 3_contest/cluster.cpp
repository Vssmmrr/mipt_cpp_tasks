#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

const int kUnknown = -1;

typedef size_t vertex_t;

struct Edge {
    vertex_t from;
    vertex_t to;
    bool is_deleted = false;
    size_t back_edge_index = 0;
};

//not oriented
class Graph {
public:
    virtual ~Graph() = default;

    virtual vertex_t GetNumVertices() const = 0;
    virtual vertex_t GetNumEdges() const = 0;

    virtual void AddEdge(vertex_t from, vertex_t to) = 0;

    virtual std::vector<Edge> GetAdjacentVertices(vertex_t vertex) const = 0;

    virtual void Transpose() = 0;
};

class AdjacencyListsGraph : public Graph {
public:
    explicit AdjacencyListsGraph(size_t num_vertices);
    AdjacencyListsGraph(size_t num_vertices, const std::vector<std::pair<vertex_t, vertex_t>>& edges_list);

    vertex_t GetNumVertices() const override;
    vertex_t GetNumEdges() const override;

    void AddEdge(vertex_t from, vertex_t to) override;
    void DeleteEdge(vertex_t from, size_t edge_number);

    std::vector<Edge> GetAdjacentVertices(vertex_t vertex) const override;

    void Transpose() override;

private:
    std::vector<std::vector<Edge>> adjacency_lists_;
    size_t num_vertices_ = 0;
    size_t num_edges_ = 0;
};

struct BridgeDfsStatus {
    int time;
    std::vector<int> t_in;
    std::vector<int> t_up;
    std::vector<std::pair<vertex_t, size_t>> answer; // vertex and edge number
    std::vector<int> cnt_edges_to_prev;
};

void DfsVisitBridges(const AdjacencyListsGraph& graph, vertex_t vertex, vertex_t previous, BridgeDfsStatus& status) {
    ++status.time;
    status.t_in[vertex] = status.time;
    status.t_up[vertex] = status.time;
    auto neighbours = graph.GetAdjacentVertices(vertex);
    size_t edge_no = 0;
    for (const auto& elem : neighbours) {
        if (elem.to == previous) {
            ++status.cnt_edges_to_prev[vertex];
            ++edge_no;
            continue;
        } else if (status.t_in[elem.to] != kUnknown) {
            status.t_up[vertex] = std::min(status.t_up[vertex], status.t_in[elem.to]);
        }
        if (status.t_in[elem.to] == kUnknown) {
            DfsVisitBridges(graph, elem.to, vertex, status);
            status.t_up[vertex] = std::min(status.t_up[vertex], status.t_up[elem.to]);
            if (status.t_in[vertex] < status.t_up[elem.to] && status.cnt_edges_to_prev[elem.to] == 1) {
                status.answer.push_back({vertex, edge_no});
            }
        }
        ++edge_no;
    }
}

std::vector<std::pair<vertex_t, size_t>> GetBridges(const AdjacencyListsGraph& graph) {
    const auto size = graph.GetNumVertices();
    BridgeDfsStatus status;
    status.time = 0;
    status.t_up.resize(size, kUnknown);
    status.t_in.resize(size, kUnknown);
    status.cnt_edges_to_prev.resize(size, 0);

    for (size_t root = 0; root < size; ++root) {
        if (status.t_in[root] == kUnknown) {
            DfsVisitBridges(graph, root, kUnknown, status);
        }
    }

    return status.answer;
}

struct CondensatedGraph {
    size_t num_connected_components;
    std::vector<size_t> components;
};

struct CondensateDfsStatus {
    std::vector<bool> visited;
    std::vector<size_t> components;
    size_t cur_component;
};

void CondensateDfsVisit(const AdjacencyListsGraph& graph,
                        vertex_t vertex, vertex_t previous,
                        CondensateDfsStatus& status) {
    status.visited[vertex] = true;
    status.components[vertex] = status.cur_component;
    auto neighbours = graph.GetAdjacentVertices(vertex);

    for (const auto& edge : neighbours) {
        if (edge.is_deleted || edge.to == previous) {
            continue;
        }
        if (!status.visited[edge.to]) {
            CondensateDfsVisit(graph, edge.to, vertex, status);
        }
    }
}

CondensatedGraph Condensate(const AdjacencyListsGraph& graph) {
    const size_t num_vertices = graph.GetNumVertices();

    CondensateDfsStatus status;
    status.visited.resize(num_vertices, false);
    status.components.resize(num_vertices);
    status.cur_component = -1;

    for (size_t vert = 0; vert < num_vertices; ++vert) {
        if (!status.visited[vert]) {
            ++status.cur_component;
            CondensateDfsVisit(graph, vert, kUnknown, status);
        }
    }

    return {status.cur_component + 1, status.components};
}

size_t MinEdgesToMakeDoublyConnected(AdjacencyListsGraph graph) {
    auto bridges = GetBridges(graph);
    for (auto [vert, edge_no] : bridges) {
        graph.DeleteEdge(vert, edge_no);
    }

    auto condensated_graph = Condensate(graph);

    std::vector<size_t> degrees(condensated_graph.num_connected_components, 0);
    for (const auto& bridge : bridges) {
        auto neighbours = graph.GetAdjacentVertices(bridge.first);
        ++degrees[condensated_graph.components[bridge.first]];
        ++degrees[condensated_graph.components[neighbours[bridge.second].to]];
    }

    size_t num_leaves = 0;
    for (auto degree : degrees) {
        if (degree == 1) {
            ++num_leaves;
        }
    }
    return (num_leaves % 2 == 0) ? (num_leaves / 2) : (num_leaves / 2 + 1);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertex, num_edges;
    std::cin >> num_vertex >> num_edges;

    AdjacencyListsGraph list_graph(num_vertex);
    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        std::cin >> from >> to;
        list_graph.AddEdge(from - 1, to - 1);
    }

    std::cout << MinEdgesToMakeDoublyConnected(list_graph) << "\n";

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
    adjacency_lists_[from].push_back({from, to});
    adjacency_lists_[to].push_back({to, from});
    adjacency_lists_[from].back().back_edge_index = adjacency_lists_[to].size() - 1;
    adjacency_lists_[to].back().back_edge_index = adjacency_lists_[from].size() - 1;
    ++num_edges_;
}


std::vector<Edge> AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
    return adjacency_lists_[vertex];
}

void AdjacencyListsGraph::Transpose() {
    std::vector<std::vector<Edge>> new_adj_list(num_vertices_);
    for (auto first_it = adjacency_lists_.begin(); first_it != adjacency_lists_.end(); ++first_it) {
        for (auto second_it = (*first_it).begin(); second_it != (*first_it).end(); ++second_it) {
            //new_adj_list[(*second_it).to].push_back(first_it - adjacency_lists_.begin());
        }
    }
    adjacency_lists_ = new_adj_list;
}

void AdjacencyListsGraph::DeleteEdge(vertex_t from, size_t edge_number) {
    Edge& edge = adjacency_lists_[from][edge_number];
    Edge& back_edge = adjacency_lists_[edge.to][edge.back_edge_index];

    edge.is_deleted = true;
    back_edge.is_deleted = true;
}

