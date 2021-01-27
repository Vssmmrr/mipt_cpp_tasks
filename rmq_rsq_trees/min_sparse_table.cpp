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

template <class T>
class MinCounter {
public:
    T operator()(const T& lhs, const T& rhs) const {
        return std::min(lhs, rhs);
    }
};

std::vector<size_t> FillArray(size_t num_elements, size_t first_element) {
    const size_t kMultiplier = 23;
    const size_t kAddValue = 21563;
    const size_t kBase = 16714589;

    std::vector<size_t> array(num_elements);
    array[0] = first_element;
    for (size_t i = 1; i < num_elements; ++i) {
        array[i] = (kMultiplier * array[i - 1] + kAddValue) % kBase;
    }

    return array;
}

struct Segment {
    size_t left;
    size_t right;
    size_t min;
};

Segment WorkWithSparseTable(const std::vector<size_t>& values, size_t first_left, size_t first_right,
                            size_t num_queries) {
    const size_t num_elements = values.size();

    SparseTable<size_t, MinCounter<size_t>> table(values);
    size_t cur_left = first_left;
    size_t cur_right = first_right;
    size_t cur_min = table.Query(cur_left - 1, cur_right);

    for (size_t i = 0; i < num_queries - 1; ++i) {
        size_t next_left = (17 * cur_left + 751 + cur_min + 2 * (i + 1)) % num_elements + 1;
        size_t next_right = (13 * cur_right + 593 + cur_min + 5 * (i + 1)) % num_elements + 1;

        size_t left = std::min(next_left, next_right);
        size_t right = std::max(next_left, next_right);

        cur_min = table.Query(left - 1, right);
        cur_left = next_left;
        cur_right = next_right;
    }
    return {cur_left, cur_right, cur_min};
}

int main() {
    size_t num_elements;
    size_t num_queries;
    size_t first_element;
    size_t first_left;
    size_t first_right;
    std::cin >> num_elements >> num_queries >> first_element >> first_left >> first_right;

    auto ans = WorkWithSparseTable(FillArray(num_elements, first_element), first_left, first_right, num_queries);
    std::cout << ans.left << " " << ans.right << " " << ans.min << "\n";
}
