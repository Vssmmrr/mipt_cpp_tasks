#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <array>

const int kInfinity = 10000000;

constexpr int kBoardSize = 4;

enum struct Step {
    UP, DOWN, LEFT, RIGHT
};

template <int Size>
class BoardState {
public:
    BoardState()
        : field_{0}, zero_x_(Size - 1), zero_y_(Size - 1) {
        for (int i = 0; i < Size * Size - 1; ++i) {
            field_[i] = i + 1;
        }
    }

    BoardState(const std::array<int8_t, Size * Size>& field)
        : field_(field) {
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                if (Get(j, i) == 0) {
                    zero_x_ = j;
                    zero_y_ = i;
                }
            }
        }
    }

    int8_t Get(int x, int y) const {
        return field_[y * Size + x];
    }

    bool MakeStep(Step step) {
        int delta_x;
        int delta_y;

        if (step == Step::UP) {
            delta_x = 0;
            delta_y = -1;
        } else if (step == Step::DOWN) {
            delta_x = 0;
            delta_y = 1;
        } else if (step == Step::LEFT) {
            delta_x = -1;
            delta_y = 0;
        } else {
            delta_x = 1;
            delta_y = 0;
        }

        int new_x = zero_x_ + delta_x;
        int new_y = zero_y_ + delta_y;
        if (new_x < 0 || new_x >= Size || new_y < 0 || new_y >= Size) {
            return false;
        }

        std::swap(field_[new_y * Size + new_x], field_[zero_y_ * Size + zero_x_]);
        zero_y_ = new_y;
        zero_x_ = new_x;
        //history_.push_back(step);
        return true;
    }

    int CountNumbersNotOnPlace() const {
        int cnt = 0;

        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                if (Get(j, i) != 0 && Get(j, i) != i * Size + j + 1) {
                    ++cnt;
                }
            }
        }
        return cnt;
    }

    int CountDistancesSum() const {
        int distances = 0;

        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                if (Get(j, i) != 0) {
                    int true_x = (Get(j, i) - 1) % Size;
                    int true_y = (Get(j, i) - 1) / Size;
                    distances += std::abs(j - true_x) + std::abs(i - true_y);
                }
            }
        }
        return distances;
    }

    int CountLinearConflicts() const {
        int num_conflicts = 0;
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                int8_t first_val = Get(j, i);
                if (first_val == 0) {
                    continue;
                }
                // true position
                int first_x = (first_val - 1) % Size;
                int first_y = (first_val - 1) % Size;

                for (int k = j + 1; k < Size; ++k) {
                    int8_t val = Get(k, i);
                    if (val == 0) {
                        continue;
                    }

                    int second_x = (val - 1) % Size;
                    int second_y = (val - 1) % Size;

                    if (second_y == first_y && second_x < first_x && second_y == i) {
                        ++num_conflicts;
                    }
                }

                for (int k = i + 1; k < Size; ++k) {
                    int8_t val = Get(j, k);
                    if (val == 0) {
                        continue;
                    }

                    int second_x = (val - 1) % Size;
                    int second_y = (val - 1) % Size;

                    if (second_y < first_y && second_x == first_x && second_x == j) {
                        ++num_conflicts;
                    }
                }
            }
        }
        return num_conflicts;
    }

    int CountCornerConflicts() const {
        int conflicts = 0;
        // Up left corner
        int8_t corner_val = Get(0, 0);
        if (corner_val != 1 && corner_val != 0) {
            int true_x = (corner_val - 1) % Size;
            int true_y = (corner_val - 1) / Size;
            if (Get(1, 0) == 2 && Get(0, 1) == Size + 1) {
                // Check for linear conflict
                if (true_x != 0 && true_y != 0) {
                    ++conflicts;
                }
            }
        }

        // Up right corner
        corner_val = Get(Size - 1, 0);
        if (corner_val != Size && corner_val != 0) {
            int true_x = (corner_val - 1) % Size;
            int true_y = (corner_val - 1) / Size;
            if (Get(Size - 2, 0) == Size - 1 && Get(Size - 1, 1) == 2 * Size) {
                // Check for linear conflict
                if (true_x != Size - 1 && true_y != 0) {
                    ++conflicts;
                }
            }
        }

        // Down left corner
        corner_val = Get(0, Size - 1);
        if (corner_val != Size * (Size - 1) + 1 && corner_val != 0) {
            int true_x = (corner_val - 1) % Size;
            int true_y = (corner_val - 1) / Size;
            if (Get(1, Size - 1) == Size * (Size - 1) + 2 && Get(0, Size - 2) == Size * (Size - 2) + 1) {
                // Check for linear conflict
                if (true_x != 0 && true_y != Size - 1) {
                    ++conflicts;
                }
            }
        }
        return conflicts == 0 ? 0 : 1;
    }

    int LastMoveHeuristic() const {
        return 0;

//        for (int i = 0; i < Size; ++i) {
//            if (Get(Size - 1, i) == Size * (Size - 1)) {
//                return 0;
//            }
//            if (Get(i, Size - 1) == Size * Size - 1) {
//                return 0;
//            }
//        }
//        return 2;

//        int8_t right_down_corner = Get(Size - 1, Size - 1);
//        if (right_down_corner == Size * Size - 1 || right_down_corner == Size * (Size - 1) || right_down_corner == 0) {
//            return 0;
//        }
//        return 2;
    }

    // Нечто похожее на хеш-функцию
    size_t BoardStateId() const {
        size_t id = 0;

        for (int i = 0; i < Size * Size; ++i) {
            id = id * Size * Size + field_[i];
        }
        return id;
    }

    bool operator==(const BoardState& other) const {
        for (int i = 0; i < Size * Size; ++i) {
            if (field_[i] != other.field_[i]) {
                return false;
            }
        }
        return true;
    }

//    std::vector<Step> GetHistory() const {
//        return history_;
//    }

    int CountInvariant() const {
        int inverses = 0;
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                int8_t first_val = Get(j, i);
                if (first_val == 0) {
                    continue;
                }
                for (int k = i; k < Size; ++k) {
                    for (int l = (i == k ? j + 1 : 0); l < Size; ++l) {
                        int8_t second_val = Get(l, k);
                        if (second_val != 0 && first_val > second_val) {
                            ++inverses;
                        }
                    }
                }
            }
        }

        if (Size % 2 != 0) {
            return inverses % 2;
        } else {
            return (inverses + zero_y_) % 2;
        }
    }

private:
    std::array<int8_t, Size * Size> field_;
    int zero_x_;
    int zero_y_;

    //std::vector<Step> history_;
};

struct Path {
    bool exists;
    std::vector<Step> steps;
};

struct VisitStatus {
    std::vector<Step> answer;
    std::unordered_set<size_t> visited;
    size_t finish_id;
    int priority_limit;
    int min_priority_over_limit;
    int cur_distance;
};

template<class Heuristic, class Priority>
bool IDAStarVisit(
    const BoardState<kBoardSize>& start,
    Heuristic heuristic, Priority priority,
    VisitStatus& status) {
    size_t start_id = start.BoardStateId();

    if (start_id == status.finish_id) {
        return true;
    }

    if (status.visited.count(start_id) == 0) {
        // not visited
        int cur_priority = priority(status.cur_distance, heuristic(start));

        if (cur_priority <= status.priority_limit) {
            status.visited.insert(start_id);
            status.cur_distance += 1;
            Step steps[] = {Step::UP, Step::DOWN, Step::LEFT, Step::RIGHT};
            for (const auto& step : steps) {
                BoardState<kBoardSize> neighbour = start;

                if (neighbour.MakeStep(step)) {
                    status.answer.push_back(step);
                    if (IDAStarVisit(neighbour, heuristic, priority, status)) {
//                        status.visited.erase(start_id);
//                        status.cur_distance -= 1;
                        return true;
                    }
                    status.answer.pop_back();
                }
            }
            status.visited.erase(start_id);
            status.cur_distance -= 1;
        } else {
            status.min_priority_over_limit = std::min(status.min_priority_over_limit, cur_priority);
        }
    }
    return false;
}

template<class Heuristic, class Priority>
Path SolveBoard(
    const BoardState<kBoardSize>& start, const BoardState<kBoardSize>& finish,
    Heuristic heuristic, Priority priority) {
    if (start.CountInvariant() != finish.CountInvariant()) {
        return {false, {}};
    }

    VisitStatus status;
    status.priority_limit = priority(0, heuristic(start));
    status.finish_id = finish.BoardStateId();
    while (true) {
        status.min_priority_over_limit = kInfinity;
        status.cur_distance = 0;
        if (IDAStarVisit(start, heuristic, priority, status)) {
            return {true, status.answer};
        }
        status.priority_limit = status.min_priority_over_limit;
    }
}

int Heuristic(const BoardState<kBoardSize>& state) {
    return state.CountDistancesSum() + state.LastMoveHeuristic() +
           2 * (state.CountLinearConflicts() + state.CountCornerConflicts());
}

int AStarPriority(int distance, int heuristic) {
    return distance + heuristic;
}

char GetStepLetter(Step step) {
    if (step == Step::UP) {
        return 'D';
    } else if (step == Step::DOWN) {
        return 'U';
    } else if (step == Step::LEFT) {
        return 'R';
    } else {
        return 'L';
    }
}

int main() {
    std::array<int8_t, kBoardSize * kBoardSize> board;
    for (int i = 0; i < kBoardSize * kBoardSize; ++i) {
        int value;
        std::cin >> value;
        board[i] = value;
    }

    auto solution = SolveBoard(BoardState<kBoardSize>(board), BoardState<kBoardSize>(), Heuristic, AStarPriority);
    if (!solution.exists) {
        std::cout << "-1\n";
    } else {
        std::cout << solution.steps.size() << "\n";
        for (Step step : solution.steps) {
            std::cout << GetStepLetter(step);
        }
        std::cout << "\n";
    }
}

