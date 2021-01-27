#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>


struct Meeting {
	int first_person;
	int second_person;
	int cost;
};

bool operator<(const Meeting& lhs, const Meeting& rhs) {
	return lhs.cost < rhs.cost;
}


class DsuAgency {
public:

	DsuAgency(std::vector<int> sending_cost);
	int GetTotalCost() const;
	int FindSet(int elem) const;
	void Union(const Meeting& meeting);

private:
	std::vector<int> sending_cost_;
	mutable std::vector<int> ranks_;
	mutable std::vector<int> pred_;
	std::vector<int> meetings_cost_;
	int total_cost_;
};




int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int num_spies;
	std::cin >> num_spies;
	std::vector<Meeting> meetings;

	for (int i = 0; i < num_spies; ++i) {
		for (int j = 0; j < num_spies; ++j) {
			int cost;
			std::cin >> cost;
			if (i > j) {
				meetings.push_back({ i, j, cost });
			}
		}
	}
	std::sort(meetings.begin(), meetings.end());

	std::vector<int> sending_cost(num_spies);
	for (int i = 0; i < num_spies; ++i) {
		std::cin >> sending_cost[i];
	}

	DsuAgency agency(sending_cost);

	for (const auto& meeting : meetings) {
		agency.Union(meeting);
	}
	std::cout << agency.GetTotalCost() << "\n";


	return 0;
}


DsuAgency::DsuAgency(std::vector<int> sending_cost) : sending_cost_(sending_cost), ranks_(sending_cost.size(), 0),
pred_(sending_cost.size()), meetings_cost_(sending_cost.size(), 0), total_cost_(0) {
	for (int i = 0; i < sending_cost_.size(); ++i) {
		pred_[i] = i;
		total_cost_ += sending_cost_[i];
	}
}


int DsuAgency::FindSet(int elem) const {
	if (pred_[elem] == elem) {
		return elem;
	}
	pred_[elem] = FindSet(pred_[elem]);
	return pred_[elem];
}

void DsuAgency::Union(const Meeting& meeting) {
	auto first = FindSet(meeting.first_person);
	auto second = FindSet(meeting.second_person);
	if (first == second) {
		return;
	}
	
	if (meeting.cost < sending_cost_[first] || meeting.cost < sending_cost_[second]) {
		int new_send_cost = std::min(sending_cost_[first], sending_cost_[second]);
		total_cost_ -= sending_cost_[first] + sending_cost_[second] - new_send_cost - meeting.cost;
		if (ranks_[first] < ranks_[second]) {
			pred_[first] = second;
			meetings_cost_[second] += meetings_cost_[first] + meeting.cost;
			sending_cost_[second] = new_send_cost;
		} else if (ranks_[first] >= ranks_[second]) {
			if (ranks_[first] == ranks_[second]) {
				++ranks_[first];
			}
			pred_[second] = first;
			meetings_cost_[first] += meetings_cost_[second] + meeting.cost;
			sending_cost_[first] = new_send_cost;
		}		
	}
}

int DsuAgency::GetTotalCost() const {
	return total_cost_;
}
