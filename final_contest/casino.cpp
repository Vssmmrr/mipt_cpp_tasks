#include <vector>
#include <algorithm>
#include <iostream>

template <class It, class It2, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
It2 Merge(It first_begin, It first_end, It second_begin, It second_end, It2 final, Cmp cmp = Cmp()) {
	while (first_begin != first_end && second_begin != second_end) {
		if (cmp(*second_begin, *first_begin)) {
			*final = *second_begin;
			++final; ++second_begin;
		} else {
			*final = *first_begin;
			++final; ++first_begin;
		}
	}
	final = std::copy(first_begin, first_end, final);
	final = std::copy(second_begin, second_end, final);
	return final;
}

template<class It, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
void MergeSort(It beg, It end, Cmp cmp, std::vector<typename std::iterator_traits<It>::value_type>& tmp) {
	if (end - beg <= 1) {
		return;
	}

	int size = end - beg;
	MergeSort(beg, beg + size / 2, cmp, tmp);
	MergeSort(beg + size / 2, end, cmp, tmp);
	Merge(beg, beg + size / 2, beg + size / 2, end, tmp.begin(), cmp);
	std::copy(tmp.begin(), tmp.begin() + size, beg);
}

template<class It, class Cmp = std::less<typename std::iterator_traits<It>::value_type>>
void MergeSort(It beg, It end, Cmp cmp = Cmp()) {
	std::vector<typename std::iterator_traits<It>::value_type> tmp(end - beg);
	MergeSort(beg, end, cmp, tmp);
}

std::vector<int64_t> GetNearestReorder(std::vector<int64_t> first, const std::vector<int64_t> second) {
	std::vector<std::pair<int64_t, uint64_t>> second_indices(second.size());

	for (size_t i = 0; i < second.size(); ++i) {
		second_indices[i].first = second[i];
		second_indices[i].second = i;
	}

	MergeSort(first.begin(), first.end());
	MergeSort(second_indices.begin(), second_indices.end());

	std::vector<int64_t> result(first.size());
	for (size_t i = 0; i < first.size(); ++i) {
		result[second_indices[i].second] = first[i];
	}
	return result;
}

int main() {
	size_t num_cards;
	std::cin >> num_cards;

	std::vector<int64_t> first_coords(num_cards);
	std::vector<int64_t> second_coords(num_cards);

	for (size_t i = 0; i < num_cards; ++i) {
		std::cin >> first_coords[i];
	}
	for (size_t i = 0; i < num_cards; ++i) {
		std::cin >> second_coords[i];
	}

	auto result = GetNearestReorder(first_coords, second_coords);
	uint64_t distance = 0;
	for (size_t i = 0; i < num_cards; ++i) {
		distance += std::llabs(result[i] - second_coords[i]);
	}

	std::cout << distance << "\n";
	for (size_t i = 0; i < num_cards; ++i) {
		std::cout << result[i] << " ";
	}
	std::cout << "\n";
	system("pause");
}
