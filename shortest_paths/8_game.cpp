#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

enum struct Step {
    UP, DOWN, LEFT, RIGHT
};

class BoardState {
public:
    BoardState(size_t size = 0)
        : size_(size), field_(size, std::vector<size_t>(size, 0)), zero_x_(size - 1), zero_y_(size - 1) {
        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < size_; ++j) {
                if (i == size_ - 1 && j == size_ - 1) {
                    break;
                }
                field_[i][j] = i * size_ + j + 1;
            }
        }
    }

    BoardState(size_t size, const std::vector<std::vector<size_t>>& field)
        : size_(size), field_(field) {
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                if (field_[i][j] == 0) {
                    zero_x_ = j;
                    zero_y_ = i;
                }
            }
        }
    }

    bool MakeStep(Step step) {
        ssize_t delta_x;
        ssize_t delta_y;

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

        ssize_t new_x = zero_x_ + delta_x;
        ssize_t new_y = zero_y_ + delta_y;
        if (new_x < 0 || new_x >= size_ || new_y < 0 || new_y >= size_) {
            return false;
        }

        std::swap(field_[new_y][new_x], field_[zero_y_][zero_x_]);
        zero_y_ = new_y;
        zero_x_ = new_x;
        history_.push_back(step);
        return true;
    }

    size_t CountNumbersNotOnPlace() const {
        size_t cnt = 0;

        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < size_; ++j) {
                if (field_[i][j] != 0 && field_[i][j] != i * size_ + j + 1) {
                    ++cnt;
                }
            }
        }
        return cnt;
    }

    ssize_t CountDistancesSum() const {
        ssize_t distances = 0;

        for (ssize_t i = 0; i < size_; ++i) {
            for (ssize_t j = 0; j < size_; ++j) {
                if (field_[i][j] != 0) {
                    ssize_t true_x = (field_[i][j] - 1) % size_;
                    ssize_t true_y = (field_[i][j] - 1) / size_;
                    distances += std::llabs(j - true_x) + std::llabs(i - true_y);
                }
            }
        }
        return distances;
    }

    // Нечто похожее на хеш-функцию
    size_t BoardStateId() const {
        size_t id = 0;

        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < size_; ++j) {
                id = id * size_ * size_ + field_[i][j];
            }
        }
        return id;
    }

    bool operator==(const BoardState& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < size_; ++j) {
                if (field_[i][j] != other.field_[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    std::vector<Step> GetHistory() const {
        return history_;
    }

    size_t CountInvariant() const {
        size_t inverses = 0;
        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < size_; ++j) {
                if (field_[i][j] == 0) {
                    continue;
                }
                for (size_t k = i; k < size_; ++k) {
                    for (size_t l = (i == k ? j + 1 : 0); l < size_; ++l) {
                        if (field_[k][l] != 0 && field_[i][j] > field_[k][l]) {
                            ++inverses;
                        }
                    }
                }
            }
        }

        if (size_ % 2 != 0) {
            return inverses % 2;
        } else {
            return (inverses + zero_y_) % 2;
        }
    }

private:
    size_t size_;
    std::vector<std::vector<size_t>> field_;
    ssize_t zero_x_;
    ssize_t zero_y_;

    std::vector<Step> history_;
};

struct PriorityState {
    ssize_t priority;
    ssize_t distance;
    BoardState board;

    bool operator>(const PriorityState& state) const {
        return priority > state.priority;
    }
};

struct Path {
    bool exists;
    std::vector<Step> steps;
};

template<class Heuristic, class Priority>
Path SolveBoard(
    const BoardState& start, const BoardState& finish,
    Heuristic heuristic, Priority priority) {
    if (start.CountInvariant() != finish.CountInvariant()) {
        return {false, {}};
    }

    std::unordered_map<size_t, ssize_t> distance;
    std::priority_queue<PriorityState, std::vector<PriorityState>, std::greater<>> state_queue;

    ssize_t start_heuristic = heuristic(start);
    state_queue.push({priority(0, start_heuristic), 0, start});
    distance[start.BoardStateId()] = 0;

    while (!state_queue.empty()) {
        auto cur_state = state_queue.top().board;
        auto dist = state_queue.top().distance;
        state_queue.pop();

        if (cur_state == finish) {
            return {true, cur_state.GetHistory()};
        }

        ssize_t cur_dist = distance[cur_state.BoardStateId()];
        if (cur_dist != dist) {
            continue;
        }

        Step steps[] = {Step::UP, Step::DOWN, Step::LEFT, Step::RIGHT};
        for (const auto& step : steps) {
            BoardState neighbour = cur_state;

            if (neighbour.MakeStep(step)) {
                size_t neighbour_id = neighbour.BoardStateId();
                if (distance.count(neighbour_id) == 0 || distance[neighbour_id] > cur_dist + 1) {
                    distance[neighbour_id] = cur_dist + 1;
                    ssize_t cur_heuristic = heuristic(cur_state);
                    state_queue.push({priority(cur_dist + 1, cur_heuristic), cur_dist + 1, neighbour});
                }
            }
        }
    }

    return {false, {}};
}

ssize_t Heuristic(const BoardState& state) {
    return state.CountDistancesSum();
    //return state.CountNumbersNotOnPlace();
}

ssize_t AStarPriority(ssize_t distance, ssize_t heuristic) {
    return distance + heuristic;
}

char GetStepLetter(Step step) {
    if (step == Step::UP) {
        return 'U';
    } else if (step == Step::DOWN) {
        return 'D';
    } else if (step == Step::LEFT) {
        return 'L';
    } else {
        return 'R';
    }
}

int main() {
    const size_t kBoardSize = 3;

    std::vector<std::vector<size_t>> board(kBoardSize, std::vector<size_t>(kBoardSize));
    for (size_t i = 0; i < kBoardSize; ++i) {
        for (size_t j = 0; j < kBoardSize; ++j) {
            std::cin >> board[i][j];
        }
    }

    auto solution = SolveBoard(BoardState(kBoardSize, board), BoardState(kBoardSize), Heuristic, AStarPriority);
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

