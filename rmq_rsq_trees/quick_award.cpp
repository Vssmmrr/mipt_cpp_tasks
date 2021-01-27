#include <iostream>
#include <vector>

class Generator {
public:
	Generator(unsigned a, unsigned b, unsigned cur = 0) : mult_(a), offset_(b), cur_(cur) {
	}

	unsigned operator()() {
		cur_ = cur_ * mult_ + offset_;
		return cur_ >> 8;
	}

private:
	unsigned int mult_;
	unsigned int offset_;
	unsigned cur_ = 0;
};

template <class T>
class PrefixSum {
public:
	
	explicit PrefixSum(int64_t size) : array_(size, 0), sums_(size, 0) {
	}

	void FillArray(int64_t m, Generator& nextRand) {
		unsigned add, l, r;
		for (unsigned int i = 0; i < m; ++i) {
			add = nextRand();
			l = nextRand();
			r = nextRand();
			if (l > r) {
				std::swap(l, r);
			}
			array_[l] += add;
			array_[r + 1] -= add;
		}
	}

	void FillPrefix() {
		unsigned value = array_[0];
		sums_[0] = value;
		for (unsigned i = 1; i < array_.size(); i++) {
			value += array_[i];
			sums_[i] = sums_[i - 1] + value;
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
	const int64_t num_soldiers = 1 << 24;
	int64_t num_stages, num_requests;
	unsigned a, b;
	std::cin >> num_stages >> num_requests >> a >> b;
	Generator nextRand(a, b);

	PrefixSum<unsigned> arr(num_soldiers);
	arr.FillArray(num_stages, nextRand);
	arr.FillPrefix();

	unsigned result = 0;
	for (unsigned i = 0; i < num_requests; ++i) {
		unsigned left = nextRand();
		unsigned right = nextRand();
		if (left > right) {
			std::swap(left, right);
		}
		result += arr.GetSum(left, right);
	}

	std::cout << result << '\n';

	system("pause");

	return 0;
}
