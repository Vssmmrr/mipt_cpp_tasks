#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>

template <class Element, class QueryType, class Op, class QueryOp>
class SegmentTree {
public:
    SegmentTree() : neutral_() {
    }

    SegmentTree(const std::vector<Element>& elems, const QueryType& neutral,
                const Op& oper = Op(), const QueryOp query_oper = QueryOp())
        : tree_(2 * GetNearestTwoPow(elems.size()) - 1), oper_(oper), query_oper_(query_oper), neutral_(neutral) {
        const auto num_elems = elems.size();
        size_t offset = tree_.size() / 2;

        for (size_t i = 0; i < num_elems; ++i) {
            tree_[i + offset] = elems[i];
        }

        for (int64_t i = offset - 1; i >= 0; --i) {
            tree_[i] = oper_(tree_[Left(i)], tree_[Right(i)]);
        }
    }

    QueryType Query(int64_t left, int64_t right, const QueryType& query_value) const {
        left += tree_.size() / 2;
        right += tree_.size() / 2 - 1;
        QueryType left_res = neutral_;
        QueryType right_res = neutral_;

        while (left < right) {
            if (left == Right(Parent(left))) {
                left_res = query_oper_(left_res, tree_[left](query_value));
            }
            left = Parent(left + 1);

            if (right == Left(Parent(right))) {
                right_res = query_oper_(tree_[right](query_value), right_res);
            }
            right = Parent(right - 1);
        }

        if (left == right) {
            left_res = query_oper_(left_res, tree_[left](query_value));
        }

        return query_oper_(left_res, right_res);
    }


private:
    std::vector<Element> tree_;
    Op oper_;
    QueryOp query_oper_;
    QueryType neutral_;

    size_t GetNearestTwoPow(size_t num) {
        int64_t pow = 0;
        while (num > 0) {
            num >>= 1;
            ++pow;
        }

        return 1ULL << pow;
    }

    static int64_t Left(int64_t idx) {
        return 2 * idx + 1;
    }

    static int64_t Right(int64_t idx) {
        return 2 * idx + 2;
    }

    static int64_t Parent(int64_t idx) {
        return (idx - 1) / 2;
    }
};

// using set is slower: merge O(NlogN) vs O(N)
class LessOrEqualCounter {
public:
    LessOrEqualCounter() = default;

    explicit LessOrEqualCounter(const std::vector<size_t>& sorted_values) : sorted_values_(sorted_values) {
    }

    explicit LessOrEqualCounter(std::vector<size_t>&& sorted_values) : sorted_values_(std::move(sorted_values)) {
    }

    size_t operator()(size_t upper_bound) const {
        // find first greater number using binary search
        auto location = std::upper_bound(sorted_values_.begin(), sorted_values_.end(), upper_bound);
        return location - sorted_values_.begin();
    }

    static LessOrEqualCounter Union(const LessOrEqualCounter& first, const LessOrEqualCounter& second) {
        std::vector<size_t> result_values(first.sorted_values_.size() + second.sorted_values_.size());
        std::merge(first.sorted_values_.begin(), first.sorted_values_.end(),
                   second.sorted_values_.begin(), second.sorted_values_.end(),
                   result_values.begin());

        return LessOrEqualCounter(std::move(result_values));
    }

private:
    std::vector<size_t> sorted_values_;
};

struct Unioner {
public:
    LessOrEqualCounter operator()(const LessOrEqualCounter& first, const LessOrEqualCounter& second) const {
        return LessOrEqualCounter::Union(first, second);
    }
};

class CorrectSubsequenceCounter {
public:
    CorrectSubsequenceCounter(const std::string& sequence) {
        const char kOpenBracket = '(';

        std::stack<size_t> open_brackets_positions;
        std::vector<std::pair<size_t, size_t>> brackets_pairs;
        for (size_t i = 0, len = sequence.size(); i < len; ++i) {
            if (sequence[i] == kOpenBracket) {
                open_brackets_positions.push(i);
            } else {
                if (!open_brackets_positions.empty()) {
                    brackets_pairs.push_back({open_brackets_positions.top(), i});
                    open_brackets_positions.pop();
                }
            }
        }

        std::sort(brackets_pairs.begin(), brackets_pairs.end());

        std::vector<LessOrEqualCounter> close_brackets_positions;
        close_brackets_positions.reserve(brackets_pairs.size());
        open_brackets_positions_.reserve(brackets_pairs.size());
        for (const auto& [left, right] : brackets_pairs) {
            open_brackets_positions_.push_back(left);
            close_brackets_positions.push_back(LessOrEqualCounter({right}));
        }
        SegmentTree<LessOrEqualCounter, size_t, Unioner, std::plus<>> tree(close_brackets_positions, 0);
        close_brackets_positions_ = std::move(tree);
    }

    size_t Query(size_t left, size_t right) const {
        auto start = std::lower_bound(open_brackets_positions_.begin(), open_brackets_positions_.end(), left);
        if (start != open_brackets_positions_.end()) {
            return 2 * close_brackets_positions_.Query(start - open_brackets_positions_.begin(), open_brackets_positions_.size(), right - 1);
        } else {
            return 0;
        }
    }

public:
    SegmentTree<LessOrEqualCounter, size_t, Unioner, std::plus<>> close_brackets_positions_;
    std::vector<size_t> open_brackets_positions_;
};

int main() {
    std::string brackets;
    std::cin >> brackets;

    CorrectSubsequenceCounter counter(brackets);

    size_t num_queries;
    std::cin >> num_queries;
    for (size_t i = 0; i < num_queries; ++i) {
        size_t left;
        size_t right;
        std::cin >> left >> right;
        std::cout << counter.Query(left - 1, right) << "\n";
    }
}

