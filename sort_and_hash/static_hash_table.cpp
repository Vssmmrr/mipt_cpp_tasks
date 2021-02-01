#include <functional>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <vector>

template <class T, class Hash, class Equal = std::equal_to<T>>
class StaticHashTable {
public:
    template<class Iterator>
    StaticHashTable(Iterator begin, Iterator end) : table_(std::distance(begin, end)) {
        std::mt19937 generator;
        const size_t size = table_.size();

        primary_hash_ = BuildPrimaryHash(begin, end, generator);

        // Separate elements into buckets
        std::vector<std::vector<T>> buckets(size);
        for (auto it = begin; it != end; ++it) {
            buckets[primary_hash_(*it) % size].push_back(*it);
        }

        // Store elements to table
        secondary_hashes_.resize(size);
        for (size_t i = 0; i < size; ++i) {
            secondary_hashes_[i] = BuildSecondaryHash(buckets[i].begin(), buckets[i].end(), generator);

            const size_t bucket_size = buckets[i].size() * buckets[i].size();
            std::vector<std::optional<T>>& bucket = table_[i];
            bucket.resize(bucket_size);

            for (T& elem : buckets[i]) {
                const size_t hash_id = secondary_hashes_[i](elem) % bucket_size;
                bucket[hash_id] = std::move(elem);
            }
        }
    }

    bool Find(const T& key) const {
        const size_t bucket_no = primary_hash_(key) % table_.size();
        const std::vector<std::optional<T>>& bucket = table_[bucket_no];
        if (!table_[bucket_no].empty()) {
            const size_t cell_no = secondary_hashes_[bucket_no](key) % bucket.size();
            if (bucket[cell_no].has_value() && equal_(key, *bucket[cell_no])) {
                return true;
            }
        }
        return false;
    }

private:
    static bool BucketDistributionIsOptimal(const std::vector<size_t>& bucket_sizes, const size_t table_size) {
        size_t sum_squares = 0;
        for (auto size : bucket_sizes) {
            sum_squares += size * size;
        }
        return sum_squares <= kMaxScale * table_size;
    }

    template <class Iterator>
    static Hash BuildPrimaryHash(Iterator begin, Iterator end, std::mt19937& generator) {
        const size_t size = std::distance(begin, end);
        std::vector<size_t> bucket_sizes(size);
        Hash primary_hash = Hash::MakeHash(generator);

        while (true) {
            std::fill(bucket_sizes.begin(), bucket_sizes.end(), 0);

            for (auto it = begin; it != end; ++it) {
                ++bucket_sizes[primary_hash(*it) % size];
            }
            if (BucketDistributionIsOptimal(bucket_sizes, size)) {
                break;
            }
            primary_hash = Hash::MakeHash(generator);
        }
        return primary_hash;
    }

    template <class Iterator>
    static Hash BuildSecondaryHash(Iterator begin, Iterator end, std::mt19937& generator) {
        const size_t elements_count = std::distance(begin, end);
        const size_t bucket_size = elements_count * elements_count;
        std::vector<bool> cell_full(bucket_size, false);
        Hash hash = Hash::MakeHash(generator);

        while (true) {
            std::fill(cell_full.begin(), cell_full.end(), false);

            bool has_collisions = false;
            for (auto it = begin; it != end && !has_collisions; ++it) {
                size_t index = hash(*it) % bucket_size;
                if (cell_full[index]) {
                    has_collisions = true;
                }
                cell_full[index] = true;
            }

            if (!has_collisions) {
                break;
            }
            hash = Hash::MakeHash(generator);
        }

        return hash;
    }

private:
    static const size_t kMaxScale = 4;

    std::vector<std::vector<std::optional<T>>> table_;
    Hash primary_hash_;
    std::vector<Hash> secondary_hashes_;
    Equal equal_;
};

template <class T>
class HashFunction {
public:
    HashFunction() = default;

    HashFunction(size_t initial_value, size_t multiplier) : initial_value_(initial_value), multiplier_(multiplier) {
    }

    size_t operator()(const T& value) const {
        return (initial_value_ + value * multiplier_) % kPrimeNumber;
    }

    template <class Generator>
    static HashFunction MakeHash(Generator& generator) {
        std::uniform_int_distribution<size_t> distr(1, kPrimeNumber - 1);
        return HashFunction(distr(generator), distr(generator));
    }

private:
    static const size_t kPrimeNumber = 2000000011;
    size_t initial_value_ = 1;
    size_t multiplier_ = 1;
};

template <>
class HashFunction<std::string> {
public:
    HashFunction() = default;

    HashFunction(size_t initial_value, size_t multiplier) : initial_value_(initial_value), multiplier_(multiplier) {
    }

    size_t operator()(const std::string& value) const {
        size_t hash = initial_value_;

        for (auto symb : value) {
            hash = (hash * multiplier_ + symb) % kPrimeNumber;
        }
        return hash;
    }

    template <class Generator>
    static HashFunction MakeHash(Generator& generator) {
        std::uniform_int_distribution<size_t> distr(1, kPrimeNumber - 1);
        return HashFunction(distr(generator), distr(generator));
    }

private:
    static const size_t kPrimeNumber = 2000000011;
    size_t initial_value_ = 1;
    size_t multiplier_ = 1;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t strings_number;
    std::cin >> strings_number;
    std::vector<std::string> strings(strings_number);
    for (size_t i = 0; i < strings_number; ++i) {
        std::cin >> strings[i];
    }

    StaticHashTable<std::string, HashFunction<std::string>> set(strings.begin(), strings.end());
    size_t queries_number;
    std::cin >> queries_number;
    std::string query;
    for (size_t i = 0; i < queries_number; ++i) {
        std::cin >> query;
        if (set.Find(query)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
}
