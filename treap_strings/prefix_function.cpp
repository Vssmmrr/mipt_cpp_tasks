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

int main() {
	std::string str;
	std::cin >> str;
	auto result = PrefixFunction(str);
	for (const auto& elem : result) {
		std::cout << elem << ' ';
	}
	std::cout << '\n';

	//system("pause");
	return 0;
}
