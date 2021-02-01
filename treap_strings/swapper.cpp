#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include <random>
#include <fstream>

template <class ValueT>
class Op {
public:
    const static ValueT kNeutral = 0;

    ValueT operator()(const ValueT& lhs, const ValueT& rhs) const {
        return lhs + rhs;
    }
};

template <class PriorityT, class ValueT, class Op>
class Swapper {
public:
    using KeyT = size_t;

    struct PriorityValue {
        PriorityT priority;
        ValueT value;
    };

    Swapper() = default;

    ~Swapper() {
        delete odd_root_;
        delete even_root_;
    }

    Swapper(const Swapper& other) = delete;
    Swapper& operator=(const Swapper& other) = delete;

    Swapper(Swapper&& other) {
        std::swap(odd_root_, other.odd_root_);
        std::swap(even_root_, other.even_root_);
    }

    Swapper& operator=(Swapper&& other) {
        std::swap(odd_root_, other.odd_root_);
        std::swap(even_root_, other.even_root_);
        return *this;
    }

    // Array of PriorityValue
    template <class FwdIt>
    Swapper(FwdIt begin, FwdIt end) {
        std::vector<PriorityValue> even_elements;
        std::vector<PriorityValue> odd_elements;

        bool is_odd = false;
        for (FwdIt it = begin; it != end; ++it, is_odd = !is_odd) {
            if (is_odd) {
                odd_elements.push_back(*it);
            } else {
                even_elements.push_back(*it);
            }
        }

        even_root_ = BuildFromArray(even_elements.begin(), even_elements.end(), oper_);
        odd_root_ = BuildFromArray(odd_elements.begin(), odd_elements.end(), oper_);
    }

    bool Find(const KeyT& key) const {
        if (key % 2 == 0) {
            return Find(even_root_, key / 2);
        } else {
            return Find(odd_root_, key / 2);
        }
    }

    uint64_t Size() const {
        return Size(even_root_) + Size(odd_root_);
    }

    ValueT DoOper(const KeyT& left_bound, const KeyT& right_bound) const {
        auto even_result = DoOper(even_root_, (left_bound + 1) / 2, (right_bound + 1) / 2, oper_);
        auto odd_result = DoOper(odd_root_, left_bound / 2, right_bound / 2, oper_);
        return oper_(even_result, odd_result);
    }

    void Swap(const KeyT& left_bound, const KeyT& right_bound) {
        auto [even_inf_left, even_left_inf] = Split(even_root_, (left_bound + 1) / 2, oper_);
        auto [even_left_right, even_right_inf] = Split(even_left_inf, (right_bound + 1) / 2 - (left_bound + 1) / 2, oper_);

        auto [odd_inf_left, odd_left_inf] = Split(odd_root_, left_bound / 2, oper_);
        auto [odd_left_right, odd_right_inf] = Split(odd_left_inf, right_bound / 2 - left_bound / 2, oper_);

        even_root_ = Merge(even_inf_left, Merge(odd_left_right, even_right_inf, oper_), oper_);
        odd_root_ = Merge(odd_inf_left, Merge(even_left_right, odd_right_inf, oper_), oper_);
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

    Node* even_root_ = nullptr;
    Node* odd_root_ = nullptr;
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
    static Node* BuildFromArray(FwdIt begin, FwdIt end, const Op& oper) {
        Node* root = nullptr;
        Node* last_added = nullptr;
        for (auto it = begin; it != end; ++it) {
            Node* new_node = new Node(0, it->priority, it->value);
            while (last_added != nullptr && last_added->priority < new_node->priority) {
                last_added = last_added->parent;
                UpdateNode(last_added, oper);
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
            UpdateNode(last_added, oper);
        }
        while (last_added != nullptr) {
            last_added = last_added->parent;
            UpdateNode(last_added, oper);
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
    size_t num_elements;
    size_t num_queries;
    std::cin >> num_elements >> num_queries;

    size_t no = 1;
    while (num_elements > 0) {
        std::cout << "Swapper " << no << ":\n";
        ++no;
        std::vector<Swapper<size_t, ssize_t, Op<ssize_t>>::PriorityValue> elements(num_elements);
        for (size_t i = 0; i < num_elements; ++i) {
            std::cin >> elements[i].value;
            elements[i].priority = rand();
        }

        Swapper<size_t, ssize_t, Op<ssize_t>> swapper(elements.begin(), elements.end());

        for (size_t i = 0; i < num_queries; ++i) {
            size_t oper;
            size_t left;
            size_t right;
            std::cin >> oper >> left >> right;

            if (oper == 1) {
                if ((right - left) % 2 == 0) {
                    return 1;
                }
                swapper.Swap(left - 1, right);
            } else {
                std::cout << swapper.DoOper(left - 1, right) << "\n";
            }
        }
        std::cout << "\n";
        std::cin >> num_elements >> num_queries;
    }

    return 0;
}
