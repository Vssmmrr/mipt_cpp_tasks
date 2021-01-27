#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>

const int kUnknown = -1;
const int kMinChildren = 2;

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
	std::unordered_set<int> answer;
};

void Dfs(const AdjacencyListsGraph& graph, int vertex, int parent, DfsStatus& status) {
	++status.time;
	status.t_in[vertex] = status.time;
	status.t_up[vertex] = status.time;
	int n_children = 0;
	auto neighbours = graph.GetAdjacentVertices(vertex);
	for (const auto& elem : neighbours) {
		if (status.t_in[elem] != kUnknown && parent != elem) {
			status.t_up[vertex] = std::min(status.t_up[vertex], status.t_in[elem]);
		}
		if (status.t_in[elem] == kUnknown) {
			++n_children;
			Dfs(graph, elem, vertex, status);
			status.t_up[vertex] = std::min(status.t_up[vertex], status.t_up[elem]);
			if (status.t_in[vertex] <= status.t_up[elem] && parent != kUnknown) {
				status.answer.insert(vertex);
			}
		}
	}
	if (parent == kUnknown && n_children >= kMinChildren) {
		status.answer.insert(vertex);
	}
}

std::vector<vertex_t> GetCutVertices(const AdjacencyListsGraph& graph) {
	const auto size = graph.GetNumVertices();
	DfsStatus status;
	status.time = 0;
	status.t_up.resize(size, kUnknown);
	status.t_in.resize(size, kUnknown);

	for (size_t root = 0; root < size; ++root) {
		if (status.t_in[root] == kUnknown) {
			Dfs(graph, root, kUnknown, status);
		}
	}

	return { status.answer.begin(), status.answer.end() };
}


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t num_skyscrapes, num_edges;
	std::cin >> num_skyscrapes >> num_edges;

	AdjacencyListsGraph list_graph(num_skyscrapes + num_edges);
	for (size_t i = 0; i < num_edges; ++i) {
		vertex_t first, second, third;
		std::cin >> first >> second >> third;
		list_graph.AddEdge(num_skyscrapes + i, first - 1);
		list_graph.AddEdge(num_skyscrapes + i, second - 1);
		list_graph.AddEdge(num_skyscrapes + i, third - 1);
	}

	auto cut_vertices = GetCutVertices(list_graph);
	std::sort(cut_vertices.begin(), cut_vertices.end());
	auto ptr_pillows = cut_vertices.begin();
	while (ptr_pillows != cut_vertices.end() && (*ptr_pillows) < num_skyscrapes) {
		++ptr_pillows;
	}

	size_t num_pillows = std::distance(ptr_pillows, cut_vertices.end());

	std::cout << num_pillows << '\n';
	for (auto it = ptr_pillows; it != cut_vertices.end(); ++it) {
		std::cout << (*it) + 1 - num_skyscrapes << ' ';
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
