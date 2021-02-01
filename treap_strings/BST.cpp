#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Information {
	int64_t parent;
	int64_t left;
	int64_t right;
};

template <class KeyT, class PriorityT>
class Treap {
public:
	struct KeyPriority {
		KeyT key;
		PriorityT priority;

		bool operator<(const KeyPriority& other) const {
			if (key != other.key) {
				return key < other.key;
			}
			return priority < other.priority;
		}
	};

	Treap() = default;

	template <class FwdIt>
	Treap(FwdIt begin, FwdIt end) : root_(nullptr) {
		if (std::is_sorted(begin, end, [](const auto& x, const auto& y) {
			return x.key < y.key;
		})) {
			root_ = BuildFromSorted(begin, end);
			return;
		}
		for (auto it = begin; it != end; ++it) {
			Insert(it->key, it->priority);
		}
	}

	bool Find(const KeyT& key) const {
		return Find(root_, key);
	}

	bool Insert(const KeyT& key, const PriorityT& priority) {
		if (!Find(key)) {
			root_ = Insert(root_, key, priority);
			return true;
		}
		return false;
	}

	bool Erase(const KeyT& key) {
		if (Find(key)) {
			root_ = Erase(root_, key);
			return true;
		}
		return false;
	}

	void TreeTraversal(std::vector<Information>& nodes, int64_t parent_number = 0) const {
		TreeTraversal(root_, nodes, parent_number);
	}

	std::pair<bool, KeyT> Next(const KeyT& key) const {
		return ExistKey(Next(root_, key));
	}

	std::pair<bool, KeyT> Prev(const KeyT& key) const {
		return ExistKey(Prev(root_, key));
	}

	std::pair<bool, KeyT> KStatistic(uint64_t k) const {
		return ExistKey(KStatistic(root_, k));
	}

private:
	struct Node {
		KeyT key;
		PriorityT priority;
		uint64_t size;
		int64_t number;
		Node* left;
		Node* right;
		Node* parent;

		Node(const KeyT& key, const PriorityT& priority)
			: key(key), priority(priority), size(1), left(nullptr), right(nullptr), parent(nullptr) {
		}

		~Node() {
			delete left;
			delete right;
		}
	};

	Node* root_;

	static uint64_t Size(Node* node) {
		return node ? node->size : 0;
	}

	static void UpdateNode(Node* node) {
		if (node) {
			node->size = 1 + Size(node->left) + Size(node->right);
		}
	}

	static std::pair<bool, KeyT> ExistKey(const Node* node) {
		if (node != nullptr) {
			return { true, node->key };
		}
		return { false, KeyT() };
	}

	static void SetLeft(Node* node, Node* new_left) {
		if (node) {
			node->left = new_left;
		}
		if (new_left) {
			new_left->parent = node;
		}
		UpdateNode(node);
	}

	static void SetRight(Node* node, Node* new_right) {
		if (node) {
			node->right = new_right;
		}
		if (new_right) {
			new_right->parent = node;
		}
		UpdateNode(node);
	}

	static std::pair<Node*, Node*> Split(Node* treap, const KeyT& key) {
		if (treap == nullptr) {
			return { nullptr, nullptr };
		}
		if (treap->key < key) {
			auto two_trees = Split(treap->right, key);
			SetRight(treap, two_trees.first);
			return { treap, two_trees.second };
		}
		else {
			auto two_trees = Split(treap->left, key);
			SetLeft(treap, two_trees.second);
			return { two_trees.first, treap };
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

	static Node* Find(Node* node, const KeyT& key) {
		while (node != nullptr) {
			if (node->key == key) {
				return node;
			}
			if (node->key > key) {
				node = node->left;
			}
			else {
				node = node->right;
			}
		}
		return nullptr;
	}

	static Node* Insert(Node* root, const KeyT& key, const PriorityT& priority) {
		if (Find(root, key)) {
			return root;
		}
		auto two_nodes = Split(root, key);
		two_nodes.first = Merge(two_nodes.first, new Node(key, priority));
		root = Merge(two_nodes.first, two_nodes.second);
		return root;
	}

	static Node* Erase(Node* root, const KeyT& key) {
		auto[less_tree, ge_tree] = Split(root, key);
		auto[equal_tree, greater_tree] = Split(ge_tree, key + 1); // x + 0
		delete equal_tree;
		return Merge(less_tree, greater_tree);
	}

	template <class FwdIt>
	static Node* BuildFromSorted(FwdIt begin, FwdIt end) {
		Node* root = nullptr;
		Node* last_added = nullptr;
		for (auto it = begin; it != end; ++it) {
			Node* new_node = new Node(it->key, it->priority);
			while (last_added != nullptr && last_added->priority < new_node->priority) {
				last_added = last_added->parent;
			}
			if (last_added == nullptr) {
				SetLeft(new_node, root);
				root = new_node;
			}
			else {
				SetLeft(new_node, last_added->right);
				SetRight(last_added, new_node);
			}
			last_added = new_node;
		}
		return root;
	}

	static void TreeTraversal(const Node* node, std::vector<Information>& nodes, int64_t parent_number = 0) {
		if (node == nullptr) {
			return;
		}
		if (node->left == nullptr) {
			nodes[node->key.second].left = 0;
		}
		else {
			nodes[node->key.second].left = node->left->key.second;
		}
		if (node->right == nullptr) {
			nodes[node->key.second].right = 0;
		}
		else {
			nodes[node->key.second].right = node->right->key.second;
		}
		nodes[node->key.second].parent = parent_number;
		TreeTraversal(node->left, nodes, node->key.second);
		TreeTraversal(node->right, nodes, node->key.second);
	}

	static const Node* Next(const Node* node, const KeyT& key) {
		const Node* upper_bound = nullptr;
		while (node != nullptr) {
			if (node->key > key) {
				upper_bound = node;
				node = node->left;
			}
			else {
				node = node->right;
			}
		}

		return upper_bound;
	}

	static const Node* Prev(const Node* node, const KeyT& key) {
		const Node* lower_bound = nullptr;
		while (node != nullptr) {
			if (node->key < key) {
				lower_bound = node;
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		return lower_bound;
	}

	static size_t GetSize(const Node* node) {
		if (node) {
			return node->size;
		}
		else {
			return 0;
		}
	}

	static const Node* KStatistic(const Node* node, uint64_t k) {
		if (node == nullptr) {
			return nullptr;
		}
		const uint64_t left_size = GetSize(node->left);
		if (left_size + 1 == k) {
			return node;
		}
		else if (left_size + 1 < k) {
			return KStatistic(node->right, k - 1 - left_size);
		}
		else {
			return KStatistic(node->left, k);
		}

	}
};


int main() {
	Treap<int64_t, uint64_t> treap;
	std::string command;
	while (std::cin >> command) {
		if (command == "insert") {
			int64_t key;
			std::cin >> key;
			treap.Insert(key, rand());
		}
		else if (command == "delete") {
			int64_t key;
			std::cin >> key;
			treap.Erase(key);
		}
		else if (command == "exists") {
			int64_t key;
			std::cin >> key;
			std::cout << (treap.Find(key) ? "true\n" : "false\n");
		}
		else if (command == "next") {
			int64_t key;
			std::cin >> key;
			auto answer = treap.Next(key);
			if (answer.first) {
				std::cout << answer.second << '\n';
			}
			else {
				std::cout << "none\n";
			}
		}
		else if (command == "prev") {
			int64_t key;
			std::cin >> key;
			auto answer = treap.Prev(key);
			if (answer.first) {
				std::cout << answer.second << '\n';
			}
			else {
				std::cout << "none\n";
			}
		}
		else if (command == "kth") {
			uint64_t k;
			std::cin >> k;
			auto answer = treap.KStatistic(k);
			if (answer.first) {
				std::cout << answer.second << '\n';
			}
			else {
				std::cout << "none\n";
			}
		}
	}

	system("pause");
	return 0;
}
