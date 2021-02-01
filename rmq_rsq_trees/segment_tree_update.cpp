#include <vector>
#include <cmath>
#include <iostream>

template <class Element, class Op, class GroupOp>
class SegmentTree {
public:
    SegmentTree(const std::vector<Element>& elems, const Element& neutral, const Op& oper = Op(), const GroupOp& group_oper = GroupOp())
        : tree_(2 * GetNearestTwoPow(elems.size()) - 1, neutral), group_deltas_(tree_),
          oper_(oper), neutral_(neutral), group_oper_(group_oper) {
        const auto num_elems = elems.size();
        size_t offset = tree_.size() / 2;

        for (size_t i = 0; i < num_elems; ++i) {
            tree_[i + offset] = elems[i];
        }

        for (int64_t i = offset - 1; i >= 0; --i) {
            tree_[i] = oper_(tree_[Left(i)], tree_[Right(i)]);
        }
    }

    Element Query(size_t left, size_t right) const {
        return Query(0, 0, tree_.size() / 2 + 1, left, right);
    }

    void Update(size_t left, size_t right, const Element& delta) {
        Update(0, 0, tree_.size() / 2 + 1, left, right, delta);
    }

private:
    std::vector<Element> tree_;
    std::vector<Element> group_deltas_;
    const Op oper_;
    const GroupOp group_oper_;
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

    Element Query(size_t cur_root, size_t root_left, size_t root_right,
                  size_t asked_left, size_t asked_right) const {
        if (root_left == asked_left && root_right == asked_right) {
            return tree_[cur_root];
        }

        size_t mid = (root_left + root_right) / 2;
        Element segment_delta = group_oper_(group_deltas_[cur_root], asked_right - asked_left);

        Element segment_result;
        if (asked_left >= mid) {
            // Go to right branch
            segment_result = Query(Right(cur_root), mid, root_right, asked_left, asked_right);
        } else if (asked_right <= mid) {
            // Go to left branch
            segment_result = Query(Left(cur_root), root_left, mid, asked_left, asked_right);
        } else {
            // Split to 2 branches
            Element left_branch = Query(Left(cur_root), root_left, mid, asked_left, mid);
            Element right_branch = Query(Right(cur_root), mid, root_right, mid, asked_right);

            segment_result = oper_(left_branch, right_branch);
        }
        return segment_result + segment_delta;
    }

    void Update(size_t cur_root, size_t root_left, size_t root_right,
                size_t asked_left, size_t asked_right, const Element& delta) {
        tree_[cur_root] = tree_[cur_root] + group_oper_(delta, asked_right - asked_left);

        if (root_left == asked_left && root_right == asked_right) {
            group_deltas_[cur_root] = group_deltas_[cur_root] + delta;
            return;
        }

        size_t mid = (root_left + root_right) / 2;

        if (asked_left >= mid) {
            // Go to right branch
            Update(Right(cur_root), mid, root_right, asked_left, asked_right, delta);
        } else if (asked_right <= mid) {
            // Go to left branch
            Update(Left(cur_root), root_left, mid, asked_left, asked_right, delta);
        } else {
            // Split to 2 branches
            Update(Left(cur_root), root_left, mid, asked_left, mid, delta);
            Update(Right(cur_root), mid, root_right, mid, asked_right, delta);
        }
    }
};

int main() {
    size_t num_elements;
    std::cin >> num_elements;

    std::vector<ssize_t> values(num_elements);
    for (size_t i = 0; i < num_elements; ++i) {
        std::cin >> values[i];
    }

    SegmentTree<ssize_t, std::plus<ssize_t>, std::multiplies<ssize_t>> tree(values, 0);
    size_t num_queries;
    std::cin >> num_queries;
    for (size_t i = 0; i < num_queries; ++i) {
        char cmd;
        std::cin >> cmd;

        if (cmd == 'g') {
            size_t index;
            std::cin >> index;

            std::cout << tree.Query(index - 1, index) << "\n";
        } else {
            size_t left;
            size_t right;
            ssize_t delta;
            std::cin >> left >> right >> delta;

            tree.Update(left - 1, right, delta);
        }
    }
}

