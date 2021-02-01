#include <iostream>
#include <numeric>
#include <string>
#include <vector>

const int kOneSet = 1;
typedef size_t vertex_t;

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

struct Request {
	std::string name;
	vertex_t first;
	vertex_t second;
};

struct Edge {
	vertex_t from;
	vertex_t to;
};

std::vector<bool> RequestProcessing(size_t num_vertices, std::vector<Request> request_list) {
	Dsu graph(num_vertices);
	std::vector<bool> answer;

	while (!request_list.empty()) {
		Request current_request = request_list[request_list.size() - 1];
		if (current_request.name == "cut") {
			graph.Union(current_request.first, current_request.second);
		}
		else if (current_request.name == "ask") {
			answer.push_back(graph.FindSet(current_request.first) == graph.FindSet(current_request.second));
		}
		request_list.pop_back();
	}

	return { answer.rbegin(), answer.rend() };
}


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t num_vertices, num_edges, num_operations;
	std::cin >> num_vertices >> num_edges >> num_operations;

	std::vector<Edge> edge_list(2 * num_edges);
	for (size_t i = 0; i < num_edges; ++i) {
		vertex_t from, to;
		std::cin >> from >> to;
		edge_list[i] = { from - 1, to - 1 };
		edge_list[i + num_edges] = { to - 1, from - 1 };
	}

	std::vector<Request> request_list;
	for (size_t i = 0; i < num_operations; ++i) {
		std::string operation;
		vertex_t first, second;
		std::cin >> operation >> first >> second;
		request_list.push_back({ operation, first - 1, second - 1 });
	}
	
	auto answer = RequestProcessing(num_vertices, request_list);

	for (const auto& elem : answer) {
		if (elem) {
			std::cout << "YES\n";
		}
		else {
			std::cout << "NO\n";
		}
	}

	return 0;
}

Dsu::Dsu(size_t n_sets) : count_different_sets_(n_sets), pred_(n_sets), ranks_(n_sets, 0) {
	std::iota(pred_.begin(), pred_.end(), 0);
}

Dsu::elem_t Dsu::MakeSet() {
	pred_.push_back(pred_.size());
	ranks_.push_back(0);
	++count_different_sets_;
	return pred_[pred_.size() - 1];
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
	}
	else if (ranks_[first] > ranks_[second]) {
		pred_[second] = first;
	}
	else {
		++ranks_[first];
		pred_[second] = first;
	}
}

size_t Dsu::CountDifferentSets() const {
	return count_different_sets_;
}
