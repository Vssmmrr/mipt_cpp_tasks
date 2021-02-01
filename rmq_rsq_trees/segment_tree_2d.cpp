#include <vector>
#include <iostream>
#include <limits>

template <class Element, class Op>
class SegmentTree2D {
public:
    SegmentTree2D(const std::vector<std::vector<Element>>& values, const Element& neutral_element, const Op& oper = Op())
        : neutral_element_(neutral_element), oper_(oper) {
        const auto tree_height = 2 * GetNearestTwoPow(values.size()) - 1;
        const auto tree_width = 2 * GetNearestTwoPow(values[0].size()) - 1;

        tree_.resize(tree_height, std::vector<Element>(tree_width, neutral_element_));

        Build(0, 0, tree_.size() / 2 + 1, values);
    }

    Element Query(size_t left, size_t top, size_t right, size_t bottom) {
        return Query(0, 0, tree_.size() / 2 + 1, top, bottom, left, right);
    }

    void Update(size_t x, size_t y, const Element& new_value) {
        Update(0, 0, tree_.size() / 2 + 1, y, x, new_value);
    }

private:
    std::vector<std::vector<Element>> tree_;
    const Element neutral_element_;
    const Op oper_;

    static size_t Left(size_t idx) {
        return 2 * idx + 1;
    }

    static size_t Right(size_t idx) {
        return 2 * idx + 2;
    }

    size_t GetNearestTwoPow(size_t num) {
        int64_t pow = 0;
        while (num > 0) {
            num >>= 1;
            ++pow;
        }

        return 1ULL << pow;
    }

    void Build(size_t root_y, size_t left_y, size_t right_y, const std::vector<std::vector<Element>>& values) {
        if (left_y + 1 < right_y) {
            size_t mid_y = (left_y + right_y) / 2;
            Build(Left(root_y), left_y, mid_y, values);
            Build(Right(root_y), mid_y, right_y, values);
        }
        Build(root_y, left_y, right_y, 0, 0, tree_[0].size() / 2 + 1, values);
    }

    void Build(size_t root_y, size_t left_y, size_t right_y, size_t root_x, size_t left_x, size_t right_x,
               const std::vector<std::vector<Element>>& values) {
        if (left_x + 1 < right_x) {
            size_t mid_x = (left_x + right_x) / 2;
            Build(root_y, left_y, right_y, Left(root_x), left_x, mid_x, values);
            Build(root_y, left_y, right_y, Right(root_x), mid_x, right_x, values);
            tree_[root_y][root_x] = oper_(tree_[root_y][Left(root_x)], tree_[root_y][Right(root_x)]);
        } else {
            if (left_y + 1 < right_y) {
                tree_[root_y][root_x] = oper_(tree_[Left(root_y)][root_x], tree_[Right(root_y)][root_x]);
            } else {
                if (left_y < values.size() && left_x < values[0].size()) {
                    tree_[root_y][root_x] = values[left_y][left_x];
                }
            }
        }
    }

    Element Query(size_t root_y, size_t left_y, size_t right_y,
                  size_t asked_left_y, size_t asked_right_y, size_t asked_left_x, size_t asked_right_x) {
        if (left_y == asked_left_y && right_y == asked_right_y) {
            return Query(root_y, 0, 0, tree_[0].size() / 2 + 1, asked_left_x, asked_right_x);
        }

        size_t mid_y = (left_y + right_y) / 2;
        if (asked_left_y >= mid_y) {
            return Query(Right(root_y), mid_y, right_y, asked_left_y, asked_right_y, asked_left_x, asked_right_x);
        } else if (asked_right_y <= mid_y) {
            return Query(Left(root_y), left_y, mid_y, asked_left_y, asked_right_y, asked_left_x, asked_right_x);
        } else {
            Element left_branch = Query(Left(root_y), left_y, mid_y, asked_left_y, mid_y, asked_left_x, asked_right_x);
            Element right_branch = Query(Right(root_y), mid_y, right_y, mid_y, asked_right_y, asked_left_x, asked_right_x);
            return oper_(left_branch, right_branch);
        }
    }

    Element Query(size_t root_y, size_t root_x, size_t left_x, size_t right_x,
                  size_t asked_left_x, size_t asked_right_x) {
        if (left_x == asked_left_x && right_x == asked_right_x) {
            return tree_[root_y][root_x];
        }

        size_t mid_x = (left_x + right_x) / 2;
        if (asked_left_x >= mid_x) {
            return Query(root_y, Right(root_x), mid_x, right_x, asked_left_x, asked_right_x);
        } else if (asked_right_x <= mid_x) {
            return Query(root_y, Left(root_x), left_x, mid_x, asked_left_x, asked_right_x);
        } else {
            Element left_branch = Query(root_y, Left(root_x), left_x, mid_x, asked_left_x, mid_x);
            Element right_branch = Query(root_y, Right(root_x), mid_x, right_x, mid_x, asked_right_x);
            return oper_(left_branch, right_branch);
        }
    }

    void Update(size_t root_y, size_t left_y, size_t right_y, size_t y, size_t x, const Element& new_value) {
        if (left_y + 1 < right_y) {
            size_t mid_y = (left_y + right_y) / 2;
            if (y < mid_y) {
                Update(Left(root_y), left_y, mid_y, y, x, new_value);
            } else {
                Update(Right(root_y), mid_y, right_y, y, x, new_value);
            }
        }
        Update(root_y, left_y, right_y, 0, 0, tree_[0].size() / 2 + 1, y, x, new_value);
    }

    void Update(size_t root_y, size_t left_y, size_t right_y, size_t root_x, size_t left_x, size_t right_x,
                size_t y, size_t x, const Element& new_value) {
        if (left_x + 1 < right_x) {
            size_t mid_x = (left_x + right_x) / 2;
            if (x < mid_x) {
                Update(root_y, left_y, right_y, Left(root_x), left_x, mid_x, y, x, new_value);
            } else {
                Update(root_y, left_y, right_y, Right(root_x), mid_x, right_x, y, x, new_value);
            }
            tree_[root_y][root_x] = oper_(tree_[root_y][Left(root_x)], tree_[root_y][Right(root_x)]);
        } else {
            if (left_y + 1 < right_y) {
                tree_[root_y][root_x] = oper_(tree_[Left(root_y)][root_x], tree_[Right(root_y)][root_x]);
            } else {
                tree_[root_y][root_x] = new_value;
            }
        }
    }
};

class MinCounter {
public:
    ssize_t operator()(ssize_t lhs, ssize_t rhs) const {
        return std::min(lhs, rhs);
    }
};

int main() {
    size_t height;
    size_t width;
    std::cin >> height >> width;

    std::vector<std::vector<ssize_t>> values(height, std::vector<ssize_t>(width));
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            std::cin >> values[i][j];
        }
    }

    SegmentTree2D<ssize_t, MinCounter> tree(values, std::numeric_limits<ssize_t>::max());

    size_t num_queries;
    std::cin >> num_queries;
    for (size_t i = 0; i < num_queries; ++i) {
        size_t x1, y1, x2, y2;
        std::cin >> y1 >> x1 >> y2 >> x2;
        std::cout << tree.Query(x1 - 1, y1 - 1, x2, y2) << "\n";
    }
}

