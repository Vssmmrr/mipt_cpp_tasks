#include <iostream>
#include <vector>

const int kOneSet = 1;

class Dsu {
public:
    typedef int elem_t;

    Dsu(size_t n_sets);
    elem_t MakeSet();
    elem_t FindSet(elem_t elem) const;
    void Union(elem_t first, elem_t second);
    size_t CountDifferentSets() const;

private:
    size_t count_different_sets_;
    mutable std::vector<elem_t> pred_;
    std::vector<size_t> ranks_;
};


int main() {
    size_t num_islands, num_bridges;
    std::cin >> num_islands >> num_bridges;

    Dsu islands(num_islands);
    for (size_t i = 0; i < num_bridges; ++i) {
        Dsu::elem_t from, to;
        std::cin >> from >> to;
        islands.Union(from, to);
        if (islands.CountDifferentSets() == kOneSet) {
            std::cout << i + 1 << '\n';
            break;
        }
    }

    return 0;
}

Dsu::Dsu(size_t n_sets) : count_different_sets_(n_sets), pred_(n_sets), ranks_(n_sets, 0) {
    for (elem_t i = 0; i < n_sets; ++i) {
        pred_[i] = i;
    }
}

Dsu::elem_t Dsu::MakeSet() {
    pred_.push_back(pred_.size());
    ranks_.push_back(0);
    ++count_different_sets_;
    return pred_[pred_.size() - 1];
}

Dsu::elem_t Dsu::FindSet(Dsu::elem_t elem) const {
    if (pred_[elem] == elem) {
        return elem;
    }
    pred_[elem] = FindSet(pred_[elem]);
    return pred_[elem];
}

void Dsu::Union(Dsu::elem_t first, Dsu::elem_t second) {
    first = FindSet(first);
    second = FindSet(second);
    if (first == second) {
        return;
    }
    --count_different_sets_;
    if (ranks_[first] < ranks_[second]) {
        pred_[first] = second;
    } else if (ranks_[first] > ranks_[second]) {
        pred_[second] = first;
    } else {
        ++ranks_[first];
        pred_[second] = first;
    }
}

size_t Dsu::CountDifferentSets() const {
    return count_different_sets_;
}
