#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<int64_t> PrefixFunction(std::string_view str) {
    const uint64_t str_size = str.size();
    std::vector<int64_t> prefix(str_size, 0);
    for (uint64_t i = 1; i < str_size; ++i) {
        int64_t k = prefix[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = prefix[k - 1];
        }
        if (str[i] == str[k]) {
            prefix[i] = k + 1;
        }
    }
    return prefix;
}

std::string Merge(std::string&& first, std::string_view second) {
    size_t first_part_len = std::min(first.size(), second.size());

    auto matching_part_len = PrefixFunction(second.data() + ('$' + std::string(first.end() - first_part_len, first.end()))).back();
    first += std::string(second.begin() + matching_part_len, second.end());
    return std::move(first);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t num_words;
    std::string result;
    std::cin >> num_words >> result;

    for (size_t i = 0; i < num_words - 1; ++i) {
        std::string suffix;
        std::cin >> suffix;
        result = Merge(std::move(result), suffix);
    }
    std::cout << result << "\n";

    //system("pause");
    return 0;
}
