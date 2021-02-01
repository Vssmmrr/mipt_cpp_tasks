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
            root_ = Erase(root_, key, oper_);
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

    std::pair<bool, ValueT> KStatistic(uint64_t k) const {
        return ExistValue(KStatistic(root_, k));
    }

    uint64_t Size() const {
        return Size(root_);
    }

    ValueT DoOper(const KeyT left_bound, const KeyT right_bound) const {
        return DoOper(root_, left_bound, right_bound, oper_);
    }

    void Rotate(const KeyT left_bound, const KeyT right_bound) {
        root_ = Rotate(root_, left_bound, right_bound, oper_);
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

        bool needs_rotate;

        Node(const KeyT& key, const PriorityT& priority, const ValueT& value)
            : key(key), priority(priority), size(1), value(value),
            result(value), left(nullptr), right(nullptr), parent(nullptr), needs_rotate(false) {
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

    static void PushRotate(Node* node, const Op& oper) {
        if (!node) {
            return;
        }

        if (node->left) {
            node->left->needs_rotate ^= node->needs_rotate;
        }
        if (node->right) {
            node->right->needs_rotate ^= node->needs_rotate;
        }
        if (node->needs_rotate) {
            std::swap(node->left, node->right);
            UpdateNode(node, oper);
        }
        node->needs_rotate = false;
    }

    static void UpdateNode(Node* node, const Op& oper_) {
        if (node) {
            node->size = 1 + Size(node->left) + Size(node->right);
            node->result = oper_(Result(node->left), oper_(node->value, Result(node->right)));
            node->key = Size(node->left);
        }
    }

    static std::pair<bool, ValueT> ExistValue(const Node* node) {
        if (node != nullptr) {
            return { true, node->value };
        }
        return { false, ValueT() };
    }

    static void SetLeft(Node* node, Node* new_left, Op oper_) {
        //PushRotate(node, oper_);
        if (node) {
            node->left = new_left;
        }
        if (new_left) {
            new_left->parent = node;
        }
        UpdateNode(node, oper_);
    }

    static void SetRight(Node* node, Node* new_right, Op oper_) {
        //PushRotate(node, oper_);
        if (node) {
            node->right = new_right;
        }
        if (new_right) {
            new_right->parent = node;
        }
        UpdateNode(node, oper_);
    }

    static std::pair<Node*, Node*> Split(Node* treap, const KeyT& key, const Op& oper_) {
        if (treap == nullptr) {
            return { nullptr, nullptr };
        }
        PushRotate(treap, oper_);
        if (treap->key < key) {
            auto two_trees = Split(treap->right, key - treap->key - 1, oper_);
            SetRight(treap, two_trees.first, oper_);
            SetLeft(nullptr, treap, oper_);
            SetRight(nullptr, two_trees.second, oper_);
            return { treap, two_trees.second };
        }
        else {
            auto two_trees = Split(treap->left, key, oper_);
            SetLeft(treap, two_trees.second, oper_);
            SetLeft(nullptr, two_trees.first, oper_);
            SetRight(nullptr, treap, oper_);
            return { two_trees.first, treap };
        }
    }

    static Node* Merge(Node* left_root, Node* right_root, const Op& oper) { // key(left) < key(right)
        if (!left_root) {
            UpdateNode(right_root, oper);
            return right_root;
        }
        if (!right_root) {
            UpdateNode(left_root, oper);
            return left_root;
        }
        if (left_root->priority < right_root->priority) {
            PushRotate(right_root, oper);
            SetLeft(right_root, Merge(left_root, right_root->left, oper), oper);
            return right_root;
        }
        else {
            PushRotate(left_root, oper);
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
                key -= node->key + 1;
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
        auto[less_tree, ge_tree] = Split(root, key, oper);
        auto[equal_tree, greater_tree] = Split(ge_tree, 1, oper);
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

    static Node* Rotate(Node* node, const KeyT& left_bound, const KeyT& right_bound, Op oper_) {
        auto [inf_left, left_inf] = Split(node, left_bound, oper_);
        auto [left_right, right_inf] = Split(left_inf, right_bound - left_bound, oper_);
        if (left_right) {
            left_right->needs_rotate ^= true;
        }
        return Merge(inf_left, Merge(left_right, right_inf, oper_), oper_);
    }
};


int main() {
//    std::ios_base::sync_with_stdio(false);
//    std::cin.tie(nullptr);

    using Array = Treap<size_t, ssize_t, Op<ssize_t>>;

    size_t num_elements;
    size_t num_queries;
    std::cin >> num_elements >> num_queries;

    std::vector<Array::PriorityValue> elements(num_elements);

    for (size_t i = 0; i < num_elements; ++i) {
        std::cin >> elements[i].value;
        elements[i].priority = rand();
    }

    Array array(elements.begin(), elements.end());

    for (size_t i = 0; i < num_queries; ++i) {
        size_t oper;
        size_t left;
        size_t right;

        std::cin >> oper >> left >> right;

        if (oper == 1) {
            array.Rotate(left - 1, right);
        } else if (oper == 2) {
            std::cout << array.DoOper(left - 1, right) << "\n";
        } else {
            for (int i = 0; i < num_elements; ++i) {
                std::cout << array.DoOper(i, i + 1) << " ";
            }
            std::cout << "\n";
        }
    }
    return 0;
}
