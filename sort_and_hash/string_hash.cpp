#include <iostream>
#include <random>
#include <unordered_set>

class StringHash {
public:
    StringHash() = default;

    StringHash(size_t seed) : seed_(seed) {
    }

    size_t operator()(const std::string& value) const {
        size_t hash = seed_ ^ (value.length() * kMultiplier);

        const uint64_t* data = static_cast<const uint64_t*>(static_cast<const void*>(value.data()));
        const uint64_t* end = data + value.length() / sizeof(uint64_t);

        while (data != end) {
            uint64_t elem = *data;
            ++data;

            elem *= kMultiplier;
            elem ^= elem >> kBitOffset;
            elem *= kMultiplier;

            hash ^= elem;
            hash *= kMultiplier;
        }

        const uint8_t* remainder = static_cast<const uint8_t*>(static_cast<const void*>(data));
        size_t remainder_len = value.length() % 8;
        for (size_t i = 0; i < remainder_len; ++i) {
            hash ^= static_cast<uint64_t>(remainder[i]) << (i * 8);
        }

        hash *= kMultiplier;
        hash ^= hash >> kBitOffset;
        hash *= kMultiplier;
        hash ^= hash >> kBitOffset;

        return hash;
    }

private:
    static const size_t kMultiplier = 0xc6a4a7935bd1e995ULL;
    static const size_t kBitOffset = 47;

    size_t seed_ = 0;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::unordered_set<std::string, StringHash> set;

    while (true) {
        char op;
        std::string value;
        std::cin >> op >> value;
        if (op == '+') {
            set.insert(value);
        } else if (op == '-') {
            set.erase(value);
        } else if (op == '?') {
            if (set.count(value) > 0) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        } else {
            break;
        }
    }
}

