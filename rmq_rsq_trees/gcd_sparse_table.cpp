#include <iostream>
#include <vector>

#include <cmath>

template <class Element, class Operation>
class SparseTable {
public:
    explicit SparseTable(const std::vector<Element>& values) : table_(1, values) {
        for (size_t i = 1; TwoPower(i) <= values.size(); ++i) {
            table_.push_back(BuildLayer(table_[i - 1], i, op_));
        }

        precomputed_logs_ = PrecomputeLogs(values.size());
    }

    Element Query(size_t left, size_t right) {
        size_t layer_no = Log(right - left);
        size_t offset = TwoPower(layer_no);
        return op_(table_[layer_no][left], table_[layer_no][right - offset]);
    }

private:
    static size_t TwoPower(size_t power) {
        return 1ULL << power;
    }

    static std::vector<Element> BuildLayer(const std::vector<Element>& prev_layer, size_t layer_no, Operation op) {
        const size_t offset = TwoPower(layer_no - 1);
        const size_t layer_size = prev_layer.size() - offset;
        std::vector<Element> layer(layer_size);

        for (size_t i = 0; i < layer_size; ++i) {
            layer[i] = op(prev_layer[i], prev_layer[i + offset]);
        }
        return layer;
    }

    static std::vector<size_t> PrecomputeLogs(size_t size) {
        std::vector<size_t> logs(size + 1, 0);

        for (size_t i = 1; i < size; ++i) {
            logs[i] = log2(i);
        }
        return logs;
    }

    size_t Log(size_t value) {
        return precomputed_logs_[value];
    }

private:
    std::vector<std::vector<Element>> table_;
    Operation op_;

    std::vector<size_t> precomputed_logs_;
};

size_t GCD(size_t lhs, size_t rhs) {
    if (lhs == 0) {
        return rhs;
    }
    return GCD(rhs % lhs, lhs);
}

class GCDCounter {
public:
    size_t operator()(size_t lhs, size_t rhs) const {
        return GCD(lhs, rhs);
    }
};

int main() {
    size_t num_soldiers;
    std::cin >> num_soldiers;
    std::vector<size_t> soldiers(num_soldiers);
    for (size_t i = 0; i < num_soldiers; ++i) {
        std::cin >> soldiers[i];
    }

    SparseTable<size_t, GCDCounter> table(soldiers);

    size_t num_queries;
    std::cin >> num_queries;
    for (size_t i = 0; i < num_queries; ++i) {
        size_t left;
        size_t right;
        std::cin >> left >> right;
        std::cout << table.Query(left - 1, right) << "\n";
    }
}
