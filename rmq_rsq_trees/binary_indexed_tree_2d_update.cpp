#include <vector>
#include <iostream>

template <class Element, class Op, class InverseOp>
class BinaryIndexedTree2D {
public:
    BinaryIndexedTree2D(size_t width, size_t height, const Element& neutral_element,
                        const Op& oper = Op(), const InverseOp& inverse_oper = InverseOp())
        : tree_(height, std::vector<Element>(width, neutral_element)), oper_(oper), inverse_oper_(inverse_oper),
          neutral_element_(neutral_element) {
    }

    BinaryIndexedTree2D(const std::vector<std::vector<Element>>& values, const Element& neutral_element,
                        const Op& oper = Op(), const InverseOp& inverse_oper = InverseOp())
        : tree_(values.size(), std::vector<Element>(values[0].size(), neutral_element)), oper_(oper),
          inverse_oper_(inverse_oper), neutral_element_(neutral_element) {
        const auto height = tree_.size();
        const auto width = tree_[0].size();

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                Update(j, i, values[i][j]);
            }
        }
    }

    void Update(size_t x, size_t y, const Element& delta) {
        const auto height = tree_.size();
        const auto width = tree_[0].size();

        for (ssize_t i = y; i < height; i = NextIncrement(i)) {
            for (ssize_t j = x; j < width; j = NextIncrement(j)) {
                tree_[i][j] = oper_(tree_[i][j], delta);
            }
        }
    }

    Element Query(ssize_t left, ssize_t top, ssize_t right, ssize_t bottom) {
        Element positive_impact = oper_(Query(right, bottom), Query(left - 1, top - 1));
        Element negative_impact = oper_(Query(left - 1, bottom), Query(right, top - 1));

        return inverse_oper_(positive_impact, negative_impact);
    }

private:
    std::vector<std::vector<Element>> tree_;
    const Op oper_;
    const InverseOp inverse_oper_;
    const Element neutral_element_;

    static ssize_t NextIncrement(ssize_t idx) {
        return idx | (idx + 1);
    }

    static ssize_t NextDecrement(ssize_t idx) {
        return (idx & (idx + 1)) - 1;
    }

    Element Query(ssize_t x, ssize_t y) const {
        Element result = neutral_element_;

        for (ssize_t i = y; i >= 0; i = NextDecrement(i)) {
            for (ssize_t j = x; j >= 0; j = NextDecrement(j)) {
                result = oper_(result, tree_[i][j]);
            }
        }

        return result;
    }
};

int main() {
    size_t height;
    size_t width;
    std::cin >> height >> width;

    BinaryIndexedTree2D<ssize_t, std::plus<ssize_t>, std::minus<ssize_t>> tree(height, width, 0);

    size_t num_queries;
    std::cin >> num_queries;

    for (size_t i = 0; i < num_queries; ++i) {
        int command;
        std::cin >> command;
        if (command == 1) {
            size_t x;
            size_t y;
            ssize_t delta;

            std::cin >> x >> y >> delta;
            tree.Update(x - 1, y - 1, delta);
        } else {
            size_t x1;
            size_t x2;
            size_t y1;
            size_t y2;

            std::cin >> x1 >> y1 >> x2 >> y2;
            std::cout << tree.Query(x1 - 1, y1 - 1, x2 - 1, y2 - 1) << "\n";
        }
    }
}

