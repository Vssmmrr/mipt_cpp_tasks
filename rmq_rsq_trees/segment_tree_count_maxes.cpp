#include <vector>
#include <cmath>
#include <iostream>

template <class Element, class Op>
class SegmentTree {
public:
    SegmentTree(const std::vector<Element>& elems, const Element& neutral, const Op& oper = Op())
        : tree_(2 * GetNearestTwoPow(elems.size()) - 1, neutral), oper_(oper), neutral_(neutral) {
        const auto num_elems = elems.size();
        size_t offset = tree_.size() / 2;

        for (size_t i = 0; i < num_elems; ++i) {
            tree_[i + offset] = elems[i];
        }

        for (int64_t i = offset - 1; i >= 0; --i) {
            tree_[i] = oper_(tree_[Left(i)], tree_[Right(i)]);
        }
    }

    Element Query(int64_t left, int64_t right) {
        left += tree_.size() / 2;
        right += tree_.size() / 2 - 1;
        Element left_res = neutral_;
        Element right_res = neutral_;

        while (left < right) {
            if (left == Right(Parent(left))) {
                left_res = oper_(left_res, tree_[left]);
            }
            left = Parent(left + 1);

            if (right == Left(Parent(right))) {
                right_res = oper_(right_res, tree_[right]);
            }
            right = Parent(right - 1);
        }

        if (left == right) {
            left_res = oper_(left_res, tree_[left]);
        }

        return oper_(left_res, right_res);
    }


private:
    std::vector<Element> tree_;
    const Op oper_;
    const Element neutral_;

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

struct MaximumCounter {
    size_t maximum;
    size_t count;
};

class MaximumUpdater {
public:
    MaximumCounter operator()(const MaximumCounter& first, const MaximumCounter& second) const {
        if (first.maximum < second.maximum) {
            return second;
        } else if (first.maximum > second.maximum) {
            return first;
        } else {
            return {first.maximum, first.count + second.count};
        }
    }
};

int main() {
    size_t num_elements;
    std::cin >> num_elements;

    std::vector<MaximumCounter> values(num_elements);
    for (size_t i = 0; i < num_elements; ++i) {
        std::cin >> values[i].maximum;
        values[i].count = 1;
    }

    SegmentTree<MaximumCounter, MaximumUpdater> tree(values, {0, 0});
    size_t num_queries;
    std::cin >> num_queries;
    for (size_t i = 0; i < num_queries; ++i) {
        size_t left;
        size_t right;
        std::cin >> left >> right;

        auto result = tree.Query(left - 1, right);
        std::cout << result.maximum << " " << result.count << "\n";
    }
}

