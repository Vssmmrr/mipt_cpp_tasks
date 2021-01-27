#include <iostream>
#include <vector>
#include <random>

class CounterTreap {
public:
	CounterTreap() : root_(nullptr) {
	}

	~CounterTreap() {
		delete root_;
	}

	CounterTreap(const CounterTreap& other) = delete;
	CounterTreap& operator=(const CounterTreap& other) = delete;
	CounterTreap(CounterTreap&& other) = delete;
	CounterTreap& operator=(CounterTreap&& other) = delete;

	void AddElement(int64_t value) {
		Node* node = Find(root_, value);
		if (node) {
			node->count += 1;
		} else {
			root_ = Insert(root_, value, random_());
		}
	}

	std::vector<std::pair<int64_t, uint64_t>> GetAllCounts() const {
		std::vector<std::pair<int64_t, uint64_t>> result;

		WalkAndAdd(root_, result);

		return result;
	}

private:
	struct Node {
		int64_t value;
		uint64_t count;
		uint64_t priority;

		Node* left;
		Node* right;
		Node* parent;

		Node(int64_t value, uint64_t priority) : value(value), count(1), priority(priority), left(nullptr), right(nullptr), parent(nullptr) {
		}

		~Node() {
			delete left;
			delete right;
		}
	};

	Node* root_;
	std::mt19937_64 random_;

	static void SetLeft(Node* node, Node* left) {
		if (node) {
			node->left = left;
		}
		if (left) {
			left->parent = node;
		}
	}

	static void SetRight(Node* node, Node* right) {
		if (node) {
			node->right = right;
		}
		if (right) {
			right->parent = node;
		}
	}

	static std::pair<Node*, Node*> Split(Node* treap, int64_t key) {
		if (treap == nullptr) {
			return { nullptr, nullptr };
		}
		if (treap->value < key) {
			auto [left_tree, right_tree] = Split(treap->right, key);
			SetRight(treap, left_tree);
			return { treap, right_tree };
		} else {
			auto [left_tree, right_tree] = Split(treap->left, key);
			SetLeft(treap, right_tree);
			return { left_tree, treap };
		}
	}

	static Node* Merge(Node* left_root, Node* right_root) { // key(left) < key(right)
		if (!left_root) {
			return right_root;
		}
		if (!right_root) {
			return left_root;
		}
		if (left_root->priority < right_root->priority) {
			SetLeft(right_root, Merge(left_root, right_root->left));
			return right_root;
		}
		else {
			SetRight(left_root, Merge(left_root->right, right_root));
			return left_root;
		}
	}

	static Node* Find(Node* treap, int64_t key) {
		Node* node = treap;

		while (node) {
			if (node->value == key) {
				return node;
			} else if (node->value < key) {
				node = node->right;
			} else {
				node = node->left;
			}
		}
		return nullptr;
	}

	static Node* Insert(Node* treap, int64_t key, uint64_t priority) {
		auto [left_tree, right_tree] = Split(treap, key);
		return Merge(left_tree, Merge(new Node(key, priority), right_tree));
	}

	static void WalkAndAdd(const Node* node, std::vector<std::pair<int64_t, uint64_t>>& data) {
		if (node) {
			WalkAndAdd(node->left, data);
			data.push_back({ node->value, node->count });
			WalkAndAdd(node->right, data);
		}
	}
};

int main() {
	CounterTreap counter;

	uint64_t size;
	std::cin >> size;

	for (uint64_t i = 0; i < size; ++i) {
		int64_t element;
		std::cin >> element;

		counter.AddElement(element);
	}

	auto counts = counter.GetAllCounts();
	for (auto[value, count] : counts) {
		std::cout << value << " " << count << "\n";
	}

        return 0;
}
