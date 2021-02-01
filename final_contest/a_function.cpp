#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<uint64_t> ZFunction(std::string_view str) {
	const uint64_t str_size = str.size();
	std::vector<uint64_t> z_func(str_size, 0);
	z_func[0] = str_size;

	for (uint64_t i = 1, left = 0, right = 0; i < str_size; ++i) {
		if (i < right) {
			z_func[i] = std::min(z_func[i - left], right - i);
		}
		while (z_func[i] + i < str_size && str[z_func[i]] == str[i + z_func[i]]) {
			++z_func[i];
		}
		if (i + z_func[i] > right) {
			left = i;
			right = i + z_func[i];
		}
	}

	return z_func;
}

std::vector<uint64_t> AFunction(const std::string& str) {
	auto z_function = ZFunction(str + '$' + std::string(str.rbegin(), str.rend()));

	return { z_function.rbegin(), z_function.rbegin() + str.size() };
}

int main() {
	size_t len;
	std::string str;
	std::cin >> len >> str;
	auto result = AFunction(str);
	for (const auto& elem : result) {
		std::cout << elem << ' ';
	}
	std::cout << '\n';


	return 0;
}
