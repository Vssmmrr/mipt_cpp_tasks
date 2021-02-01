#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<size_t> PrefixFunction(std::string_view str) {
    const size_t str_size = str.size();
    std::vector<size_t> prefix(str_size, 0);
    for (size_t i = 1; i < str_size; ++i) {
        ssize_t k = prefix[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = prefix[k - 1];
        }
        if (str[i] == str[k]) {
            prefix[i] = k + 1;
        }
    }
    return prefix;
}

std::vector<size_t> GetMaximumoccurenciesLengths(std::string_view base, std::string_view analysed) {
    auto prefix_function = PrefixFunction(std::string(base) + '$' + analysed.data());
    return {prefix_function.begin() + base.size() + 1, prefix_function.end()};
}

std::vector<std::string> DivideToPrefixes(std::string_view base, std::string_view to_divide) {
    auto prefixes_lengths = GetMaximumoccurenciesLengths(base, to_divide);

    std::vector<std::string> result;
    ssize_t last_element = to_divide.size() - 1;
    while (last_element >= 0) {
        if (prefixes_lengths[last_element] == 0) {
            return {};
        }

        result.emplace_back(base.begin(), base.begin() + prefixes_lengths[last_element]);
        last_element -= prefixes_lengths[last_element];
    }
    return {result.rbegin(), result.rend()};
}

int main() {
    std::string base;
    std::string best_word;
    std::cin >> base >> best_word;

    auto division = DivideToPrefixes(base, best_word);

    if (division.empty()) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
        for (const auto& word : division) {
            std::cout << word << " ";
        }
        std::cout << "\n";
    }

    //system("pause");
    return 0;
}
