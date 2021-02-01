#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <unordered_map>

typedef size_t vertex_t;
const int INF = 100000;

struct Edge {
    vertex_t from_;
    vertex_t to_;
    int length_;

    Edge(vertex_t from, vertex_t to, int length) : from_(from), to_(to), length_(length) {
    }
};


//oriented
class Graph {
public:


    virtual vertex_t GetNumVertices() const = 0;
    virtual size_t GetNumEdges() const = 0;

    virtual bool HasEdge(vertex_t from, vertex_t to) const = 0;
    virtual void AddEdge(vertex_t from, vertex_t to, int length) = 0;
    virtual void DeleteEdge(vertex_t from, vertex_t to) = 0;

    virtual std::unordered_map<vertex_t, int> GetAdjacentVertices(vertex_t vertex) const = 0;

};

class AdjacencyListsGraph : public Graph {
public:
    explicit AdjacencyListsGraph(vertex_t num_vertices);
    AdjacencyListsGraph(vertex_t num_vertices, const std::vector<Edge>& edges_list);

    vertex_t GetNumVertices() const override;
    size_t GetNumEdges() const override;

    bool HasEdge(vertex_t from, vertex_t to) const override;
    void AddEdge(vertex_t from, vertex_t to, int length) override;
    void DeleteEdge(vertex_t from, vertex_t to) override;

    std::unordered_map<vertex_t, int> GetAdjacentVertices(vertex_t vertex) const override;


private:
    std::vector<std::unordered_map<vertex_t, int>> adjacency_lists_;
    size_t num_edges_ = 0;
};

int GetShortestPath(const AdjacencyListsGraph& graph, vertex_t start, vertex_t end, int kMaxLength) {
    std::vector<bool> visited(graph.GetNumVertices(), false);
    std::vector<std::queue<vertex_t>> array_of_queues(kMaxLength + 1);
    std::vector<size_t> distance(graph.GetNumVertices(), INF);
    size_t id = 0;
    array_of_queues[id].push(start);
    int filled_queues = 1;
    distance[start] = 0;
    while (filled_queues) {
        if (!array_of_queues[id].empty()) {
            while (!array_of_queues[id].empty()) {
                vertex_t v = array_of_queues[id].front();
                array_of_queues[id].pop();
                if (visited[v]) {
                    continue;
                }
                auto neighbours = graph.GetAdjacentVertices(v);
                for (const auto&[to, weight] : neighbours) {
                    if (distance[to] > distance[v] + weight) {
                        if (array_of_queues[(id + weight) % (kMaxLength + 1)].empty()) {
                            ++filled_queues;
                        }
                        array_of_queues[(id + weight) % (kMaxLength + 1)].push(to);
                        distance[to] = distance[v] + weight;
                    }
                }

                visited[v] = true;
            }
            --filled_queues;
        }
        id = (id + 1) % (kMaxLength + 1);
    }

    if (distance[end] == INF) {
        return -1;
    } else {
        return distance[end];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_vertices, num_edges;
    std::cin >> num_vertices >> num_edges;
    vertex_t start, end;
    std::cin >> start >> end;
    std::vector<Edge> edges;
    int kMaxLength = 0;
    for (size_t i = 0; i < num_edges; ++i) {
        vertex_t from, to;
        int length;
        std::cin >> from >> to >> length;
        if (length > kMaxLength) {
            kMaxLength = length;
        }
        edges.push_back({from - 1, to - 1, length});
    }

    AdjacencyListsGraph graph(num_vertices, edges);

    int shortest_path = GetShortestPath(graph, start - 1, end - 1, kMaxLength);
    std::cout << shortest_path << '\n';

    return 0;
}

AdjacencyListsGraph::AdjacencyListsGraph(vertex_t num_vertices)
        : adjacency_lists_(num_vertices) {
}

AdjacencyListsGraph::AdjacencyListsGraph(vertex_t num_vertices, const std::vector<Edge>& edges_list)
        : AdjacencyListsGraph(num_vertices) {
    for (const auto& elem : edges_list) {
        AddEdge(elem.from_, elem.to_, elem.length_);
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
void AdjacencyListsGraph::AddEdge(vertex_t from, vertex_t to, int length) {
    if (adjacency_lists_[from].count(to) == 0) {
        adjacency_lists_[from].insert({to, length});
        ++num_edges_;
    } else {
        adjacency_lists_[from][to] = std::min(adjacency_lists_[from][to], length);
    }
}

//if there are multiple edges, it removes only one
void AdjacencyListsGraph::DeleteEdge(vertex_t from, vertex_t to) {
    if (HasEdge(from, to)) {
        adjacency_lists_[from].erase(to);
        --num_edges_;
    }
}

std::unordered_map<vertex_t, int> AdjacencyListsGraph::GetAdjacentVertices(vertex_t vertex) const {
    return adjacency_lists_[vertex];
}


