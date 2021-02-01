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

std::vector<int64_t> FindPattern(const std::string& str,const std::string& pattern) {
	auto prefix = PrefixFunction(pattern + '#' + str);
	std::vector<int64_t> result;
	const uint64_t pattern_size = pattern.size();

	for (uint64_t i = 2 * pattern_size; i < pattern_size + str.size() + 1; ++i) {
		if (prefix[i] == pattern_size) {
			result.push_back(i - 2 * pattern_size);
		}
	}
	return result;
}

int main() {
	std::string str, pattern;
	std::cin >> str >> pattern;
	auto result = FindPattern(str, pattern);
	for (const auto& elem : result) {
		std::cout << elem << ' ';
	}
	std::cout << '\n';

	system("pause");
	return 0;
}
