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

bool FindCyclicPattern(const std::string& str, const std::string& pattern) {
	std::string temp(pattern + '#' + str + str);
	auto prefix = PrefixFunction(temp);
	const uint64_t pattern_size = pattern.size();

	for (uint64_t i = 2 * pattern_size; i < pattern_size + 2 * str.size() + 1; ++i) {
		if (prefix[i] == pattern_size) {
			return temp[i];
		}
	}
	return false;
}

int main() {
	int64_t num;
	std::string str, pattern;
	std::cin >> num >> pattern >> str;
	auto first = FindCyclicPattern(str + '1', pattern);
	auto second = FindCyclicPattern(str + '0', pattern);
	
	if (first && second) {
		std::cout << "Random\n";
	} else if (first) {
		std::cout << "Yes\n";
	} else if (second) {
		std::cout << "No\n";
	}

	system("pause");
	return 0;
}
