#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int64_t GetMaxPrefixFunction(std::string_view str) {
	const uint64_t str_size = str.size();
	int64_t max = 0;
	std::vector<int64_t> prefix(str_size, 0);
	for (uint64_t i = 1; i < str_size; ++i) {
		int64_t k = prefix[i - 1];
		while (k > 0 && str[i] != str[k]) {
			k = prefix[k - 1];
		}
		if (str[i] == str[k]) {
			prefix[i] = k + 1;
		}
		max = std::max(max, prefix[i]);
	}
	return max;
}

int64_t CountDifferentSubstrings(const std::string& str) {
	const uint64_t str_size = str.size();
	std::string reversed_prefix;
	int64_t counter = 0;
	for (uint64_t i = 0; i < str_size; ++i) {
		reversed_prefix = str[i] + reversed_prefix;
		counter += i + 1 - GetMaxPrefixFunction(reversed_prefix);
	}
	return counter;
}

int main() {
	std::string str;
	std::cin >> str;
	auto result = CountDifferentSubstrings(str);
	std::cout << result << '\n';

	//system("pause");
	return 0;
}
