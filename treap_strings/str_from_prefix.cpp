#include <iostream>
#include <string>
#include <set>
#include <vector>

std::string StringFromPrefixFunction(const std::vector<int64_t>& prefix, char first_symb = 'a') {
	const uint64_t prefix_size = prefix.size();
	if (prefix_size == 0) {
		return "";
	}

	std::string str(1, first_symb);
	for (uint64_t i = 1; i < prefix_size; ++i) {
		if (prefix[i] > 0) {
			str.push_back(str[prefix[i] - 1]);
		} else {
			std::set<char> used;
			int64_t k = prefix[i - 1];
			while (k > 0) {
				used.insert(str[k]);
				k = prefix[k - 1];
			}
			used.insert(first_symb);
			uint64_t max_size = 26;
			for (uint64_t i = 0; i < max_size; ++i) {
				if (used.find(first_symb + i) == used.end()) {
					str.push_back(i + first_symb);
					break;
				}
			}
		}
	}

	return str;
}

int main() {
	int64_t size;
	std::cin >> size;
	std::vector<int64_t> prefix(size);

	for (int64_t i = 0; i < size; ++i) {
		std::cin >> prefix[i];
	}
	std::cout << StringFromPrefixFunction(prefix) << '\n';

	system("pause");
	return 0;
}
