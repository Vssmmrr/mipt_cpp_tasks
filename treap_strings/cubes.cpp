#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<size_t> ZFunction(const std::vector<size_t>& elements) {
	const size_t elements_size = elements.size();
	std::vector<size_t> z_func(elements_size, 0);
	z_func[0] = elements_size;

	for (size_t i = 1, left = 0, right = 0; i < elements_size; ++i) {
		if (i < right) {
			z_func[i] = std::min(z_func[i - left], right - i);
		}
		while (z_func[i] + i < elements_size && elements[z_func[i]] == elements[i + z_func[i]]) {
			++z_func[i];
		}
		if (i + z_func[i] > right) {
			left = i;
			right = i + z_func[i];
		}
	}

	return z_func;
}

std::vector<size_t> FindMatchesWithReflectedStringLength(std::vector<size_t> cubes) {
    cubes.push_back(0);
    cubes.insert(cubes.end(), cubes.rbegin() + 1, cubes.rend());
    return std::move(ZFunction(cubes));
}

std::vector<size_t> PossibleCubesCount(const std::vector<size_t>& cubes) {
    const auto num_cubes = cubes.size();

    auto reflected_matches_len = FindMatchesWithReflectedStringLength(cubes);

    std::vector<size_t> counts;
    for (size_t i = num_cubes / 2; i >= 1; --i) {
        if (reflected_matches_len[2 * num_cubes - 2 * i + 1] >= i) {
            counts.push_back(num_cubes - i);
        }
    }
    counts.push_back(num_cubes);

    return counts;
}

int main() {
	size_t num_cubes;
	size_t num_colors;

	std::cin >> num_cubes >> num_colors;

	std::vector<size_t> cubes(num_cubes);
	for (size_t i = 0; i < num_cubes; ++i) {
	    std::cin >> cubes[i];
	}

	auto counts = PossibleCubesCount(cubes);
	for (const auto& count: counts) {
	    std::cout << count << " ";
	}
	std::cout << "\n";
	return 0;
}
