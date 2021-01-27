#include <iostream>
#include <vector>

template <class T>
class PrefixSum {
public:
	PrefixSum(std::vector<T> arr) : array_(arr), sums_(arr.size(), 0) {
		sums_[0] = array_[0];
		for (size_t i = 1; i < array_.size(); ++i) {
			sums_[i] = sums_[i - 1] + array_[i];
		}
	}

	T GetSum(int64_t left, int64_t right) const {
		return GetSum(right) - GetSum(left - 1);
	}


private:
	std::vector<T> array_;
	std::vector<T> sums_;

	T GetSum(int64_t idx) const {
		return idx < 0 ? 0 : sums_[idx];
	}
};


int main() {
	int64_t num_soldiers, num_requests;
	std::cin >> num_soldiers;
	std::vector<int64_t> maxes(num_soldiers);
	for (int64_t i = 0; i < num_soldiers; ++i) {
		int64_t value;
		std::cin >> value;
		maxes[i] = value;
	}

	PrefixSum<int64_t> sums(maxes);
	std::cin >> num_requests;
	for (int64_t i = 0; i < num_requests; ++i) {
		int64_t left, right;
		std::cin >> left >> right;
		std::cout << sums.GetSum(left - 1, right - 1) << '\n';
	}

	system("pause");
	return 0;
}
