#include <vector>
#include <iostream>

template <class Element, class Op, class InverseOp>
class BinaryIndexedTree3D {
public:
    BinaryIndexedTree3D(size_t width, size_t height, size_t depth, const Element& neutral_element,
                        const Op& oper = Op(), const InverseOp& inverse_oper = InverseOp())
        : tree_(depth, std::vector<std::vector<Element>>(height, std::vector<Element>(width, neutral_element))),
          oper_(oper), inverse_oper_(inverse_oper), neutral_element_(neutral_element) {
    }

    BinaryIndexedTree3D(const std::vector<std::vector<std::vector<Element>>>& values, const Element& neutral_element,
                        const Op& oper = Op(), const InverseOp& inverse_oper = InverseOp())
        : tree_(values.size(), std::vector<std::vector<Element>>(values[0].size(), std::vector<Element>(values[0][0].size(), neutral_element))),
          oper_(oper), inverse_oper_(inverse_oper), neutral_element_(neutral_element) {
        const auto depth = tree_.size();
        const auto height = tree_[0].size();
        const auto width = tree_[0][0].size();

        for (size_t k = 0; k < depth; ++k) {
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    Update(j, i, values[k][i][j]);
                }
            }
        }
    }

    void Update(size_t x, size_t y, size_t z, const Element& delta) {
        const auto depth = tree_.size();
        const auto height = tree_[0].size();
        const auto width = tree_[0][0].size();

        for (size_t k = z; k < depth; k = NextIncrement(k)) {
            for (ssize_t i = y; i < height; i = NextIncrement(i)) {
                for (ssize_t j = x; j < width; j = NextIncrement(j)) {
                    tree_[k][i][j] = oper_(tree_[k][i][j], delta);
                }
            }
        }
    }

    Element Query(ssize_t left, ssize_t top, ssize_t nearest, ssize_t right, ssize_t bottom, ssize_t furthest) {
        // Include - exclude formula
        Element positive_impact = Query(right, bottom, furthest);
        positive_impact = oper_(positive_impact, Query(right, top - 1, nearest - 1));
        positive_impact = oper_(positive_impact, Query(left - 1, bottom, nearest - 1));
        positive_impact = oper_(positive_impact, Query(left - 1, top - 1, furthest));

        Element negative_impact = Query(left - 1, top - 1, nearest - 1);
        negative_impact = oper_(negative_impact, Query(right, bottom, nearest - 1));
        negative_impact = oper_(negative_impact, Query(right, top - 1, furthest));
        negative_impact = oper_(negative_impact, Query(left - 1, bottom, furthest));

        return inverse_oper_(positive_impact, negative_impact);
    }

private:
    std::vector<std::vector<std::vector<Element>>> tree_;
    const Op oper_;
    const InverseOp inverse_oper_;
    const Element neutral_element_;

    static ssize_t NextIncrement(ssize_t idx) {
        return idx | (idx + 1);
    }

    static ssize_t NextDecrement(ssize_t idx) {
        return (idx & (idx + 1)) - 1;
    }

    Element Query(ssize_t x, ssize_t y, ssize_t z) const {
        Element result = neutral_element_;

        for (ssize_t k = z; k >= 0; k = NextDecrement(k)) {
            for (ssize_t i = y; i >= 0; i = NextDecrement(i)) {
                for (ssize_t j = x; j >= 0; j = NextDecrement(j)) {
                    result = oper_(result, tree_[k][i][j]);
                }
            }
        }

        return result;
    }
};

int main() {
    size_t size;
    std::cin >> size;

    BinaryIndexedTree3D<ssize_t, std::plus<ssize_t>, std::minus<ssize_t>> tree(size, size, size, 0);

    while (true) {
        int command;
        std::cin >> command;
        if (command == 1) {
            size_t x;
            size_t y;
            size_t z;
            ssize_t delta;

            std::cin >> x >> y >> z >> delta;
            tree.Update(x, y, z, delta);
        } else if (command == 2) {
            size_t x1;
            size_t x2;
            size_t y1;
            size_t y2;
            size_t z1;
            size_t z2;

            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            std::cout << tree.Query(x1, y1, z1, x2, y2, z2) << "\n";
        } else {
            break;
        }
    }
}

