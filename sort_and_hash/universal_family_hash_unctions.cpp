#include <iostream>
#include <random>
#include <unordered_set>

class HashFunction {
public:
    HashFunction() = default;

    HashFunction(size_t initial_value, size_t multiplier) : initial_value_(initial_value), multiplier_(multiplier) {
    }

    size_t operator()(int value) const {
        return (initial_value_ + value * multiplier_) % kPrimeNumber;
    }

    template <class Generator>
    static HashFunction MakeHash(Generator& generator) {
        std::uniform_int_distribution<size_t> distr(1, kPrimeNumber - 1);
        return HashFunction(distr(generator), distr(generator));
    }

private:
    static const size_t kPrimeNumber = 2000000011;
    size_t initial_value_ = 1345355;
    size_t multiplier_ = 86456788;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::unordered_set<int, HashFunction> set;

    size_t num_operations;
    std::cin >> num_operations;
    for (size_t i = 0; i < num_operations; ++i) {
        char op;
        int value;
        std::cin >> op >> value;
        if (op == '+') {
            set.insert(value);
        } else if (op == '-') {
            set.erase(value);
        } else {
            if (set.count(value) > 0) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        }
    }
}

