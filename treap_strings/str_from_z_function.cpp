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
		}
		else {
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

std::vector<int64_t> PrefixFromZfunction(const std::vector<int64_t>& z_func) {
	const uint64_t z_size = z_func.size();
	std::vector<int64_t> prefix(z_size, 0);
	for (uint64_t i = 1; i < z_size; ++i) {
		for (int64_t delta = z_func[i] - 1; delta >= 0; --delta) {
			if (prefix[i + delta] > 0) {
				break;
			}
			prefix[i + delta] = delta + 1;
		}
	}

	return prefix;
}

int main() {
	uint64_t size;
	std::cin >> size;
	std::vector<int64_t> z_function(size);
	for (uint64_t i = 0; i < size; ++i) {
		std::cin >> z_function[i];
	}
	auto prefix_function(PrefixFromZfunction(z_function));
	std::cout << StringFromPrefixFunction(prefix_function) << '\n';


	system("pause");
	return 0;
}
