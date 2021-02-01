#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

template <class ValueT>
class Op {
public:
	const static ValueT kNeutral = std::numeric_limits<ValueT>::max();

	ValueT operator()(const ValueT& lhs, const ValueT& rhs) const {
		return std::min(lhs, rhs);
	}
};

template <class PriorityT, class ValueT, class Op>
class Treap {
public:
    using KeyT = size_t;

    struct PriorityValue {
        PriorityT priority;
        ValueT value;
    };

	Treap() : root_(nullptr) {
	}

	~Treap() {
		delete root_;
	}

    Treap(const Treap& other) = delete;
    Treap& operator=(const Treap& other) = delete;

    Treap(Treap&& other) : Treap() {
        std::swap(root_, other.root_);
    }

    Treap& operator=(Treap&& other) {
        std::swap(root_, other.root_);
        return *this;
    }

	// Array of PriorityValue
	template <class FwdIt>
	Treap(FwdIt begin, FwdIt end) : root_(nullptr) {
	    root_ = BuildFromArray(begin, end, oper_);
	}

	bool Find(const KeyT& key) const {
		return Find(root_, key);
	}

	void Insert(const KeyT& key, const PriorityT& priority, const ValueT& value) {
	    root_ = Insert(root_, key, priority, value, oper_);
	}

	bool Erase(const KeyT& key) {
		if (Find(key)) {
			root_ = Erase(root_, key);
			return true;
		}
		return false;
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

	uint64_t Size() const {
		return Size(root_);
	}

	ValueT DoOper(const KeyT left_bound, const KeyT right_bound) const {
		return DoOper(root_, left_bound, right_bound, oper_);
	}


private:
	struct Node {
		KeyT key;
		PriorityT priority;
		uint64_t size;

		ValueT value;
		ValueT result;

		Node* left;
		Node* right;
		Node* parent;

		Node(const KeyT& key, const PriorityT& priority, const ValueT& value) : key(key), priority(priority), value(value), 
			result(value), size(1), left(nullptr), right(nullptr), parent(nullptr) {
		}

		~Node() {
			delete left;
			delete right;
		}
	};

	Node* root_;
	const Op oper_ = Op();

	static uint64_t Size(Node* node) {
		return node ? node->size : 0;
	}

	static void UpdateNode(Node* node, Op oper_) {
		if (node) {
			node->size = 1 + Size(node->left) + Size(node->right);
			node->result = oper_(Result(node->left), oper_(node->value, Result(node->right)));
			node->key = Size(node->left);
		}
	}

	static std::pair<bool, KeyT> ExistKey(const Node* node) {
		if (node != nullptr) {
			return { true, node->key };
		}
		return { false, KeyT() };
	}

	static void SetLeft(Node* node, Node* new_left, Op oper_) {
		if (node) {
			node->left = new_left;
		}
		if (new_left) {
			new_left->parent = node;
		}
		UpdateNode(node, oper_);
	}

	static void SetRight(Node* node, Node* new_right, Op oper_) {
		if (node) {
			node->right = new_right;
		}
		if (new_right) {
			new_right->parent = node;
		}
		UpdateNode(node, oper_);
	}

	static std::pair<Node*, Node*> Split(Node* treap, const KeyT& key, Op oper_) {
		if (treap == nullptr) {
			return { nullptr, nullptr };
		}
		if (treap->key < key) {
			auto two_trees = Split(treap->right, key - treap->key - 1, oper_);
			SetRight(treap, two_trees.first, oper_);
			return { treap, two_trees.second };
		}
		else {
			auto two_trees = Split(treap->left, key, oper_);
			SetLeft(treap, two_trees.second, oper_);
			return { two_trees.first, treap };
		}
	}

	static Node* Merge(Node* left_root, Node* right_root, Op oper) { // key(left) < key(right)
		if (!left_root) {
		    UpdateNode(right_root, oper);
			return right_root;
		}
		if (!right_root) {
		    UpdateNode(left_root, oper);
			return left_root;
		}
		if (left_root->priority < right_root->priority) {
			SetLeft(right_root, Merge(left_root, right_root->left, oper), oper);
			return right_root;
		}
		else {
			SetRight(left_root, Merge(left_root->right, right_root, oper), oper);
			return left_root;
		}
	}

	static Node* Find(Node* node, KeyT key) {
		while (node != nullptr) {
			if (node->key == key) {
				return node;
			}
			if (node->key > key) {
				node = node->left;
			}
			else {
			    key -= node->key - 1;
				node = node->right;
			}
		}
		return nullptr;
	}

	static Node* Insert(Node* root, const KeyT& key, const PriorityT& priority, const ValueT& value, Op oper_) {
		auto two_nodes = Split(root, key, oper_);
		two_nodes.first = Merge(two_nodes.first, new Node(0, priority, value), oper_);
		root = Merge(two_nodes.first, two_nodes.second, oper_);
		return root;
	}

	static Node* Erase(Node* root, const KeyT& key, const Op& oper) {
		auto[less_tree, ge_tree] = Split(root, key);
		auto[equal_tree, greater_tree] = Split(ge_tree, key + 1); // x + 0
		delete equal_tree;
		return Merge(less_tree, greater_tree, oper);
	}

	template <class FwdIt>
	static Node* BuildFromArray(FwdIt begin, FwdIt end, Op oper) {
		Node* root = nullptr;
		Node* last_added = nullptr;
		for (auto it = begin; it != end; ++it) {
			Node* new_node = new Node(0, it->priority, it->value);
			while (last_added != nullptr && last_added->priority < new_node->priority) {
				last_added = last_added->parent;
			}
			if (last_added == nullptr) {
				SetLeft(new_node, root, oper);
				root = new_node;
			}
			else {
				SetLeft(new_node, last_added->right, oper);
				SetRight(last_added, new_node, oper);
			}
			last_added = new_node;
		}
		return root;
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

	static ValueT Result(const Node* node) {
		return node ? node->result : Op::kNeutral;
	}

	static const Node* KStatistic(const Node* node, uint64_t k) {
		if (node == nullptr) {
			return nullptr;
		}
		const uint64_t left_size = Size(node->left);
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

	static ValueT DoOper(Node* node, const KeyT& left_bound, const KeyT& right_bound, Op oper_) {
		auto [inf_left, left_inf] = Split(node, left_bound, oper_);
		auto [left_right, right_inf] = Split(left_inf, right_bound - left_bound, oper_);
		auto answer = Result(left_right);
		node = Merge(inf_left, Merge(left_right, right_inf, oper_), oper_);
		return answer;
	}
};


int main() {
	Treap<uint64_t, int64_t, Op<int64_t>> treap;
	int64_t num_operations;
	std::cin >> num_operations;

	for (int64_t i = 0; i < num_operations; ++i) {
		std::string operation;
		std::cin >> operation;
		if (operation == "+") {
			int64_t key, value;
			std::cin >> key >> value;
			treap.Insert(key, rand(), value);
		} else {
			int64_t left, right;
			std::cin >> left >> right;
			std::cout << treap.DoOper(left - 1, right) << '\n';
		}
	}
	

	//system("pause");
	return 0;
}
