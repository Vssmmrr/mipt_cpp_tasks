#include <iostream>
#include <vector>

struct Teleportation {
	int64_t from;
	int64_t departure_time;
	int64_t to;
	int64_t arrival_time;
};

const int64_t kInfinity = 1000000500;

bool Improvement(const Teleportation& teleport, std::vector<int64_t>& arrivals) {
	if (arrivals[teleport.from] <= teleport.departure_time && arrivals[teleport.to] > teleport.arrival_time) {
		arrivals[teleport.to] = teleport.arrival_time;
		return true;
	}
	return false;
}

int64_t GetMinTime(const std::vector<Teleportation>& teleportations, int64_t num_stations, int64_t start, int64_t end) {
	std::vector<int64_t> arrivals(num_stations, kInfinity);
	arrivals[start] = 0;
	for (uint64_t i = 0; i < num_stations + teleportations.size() - 1; ++i) {
		for (const auto& teleport : teleportations) {
			Improvement(teleport, arrivals);
		}
	}
	return arrivals[end];
}

int main() {
	int64_t num_stations, start, end, num_teleportations;
	std::cin >> num_stations >> start >> end >> num_teleportations;

	std::vector<Teleportation> teleportations(num_teleportations);
	for (int64_t i = 0; i < num_teleportations; ++i) {
		int64_t from, depature, to, arrival;
		std::cin >> from >> depature >> to >> arrival;
		teleportations[i] = { from - 1, depature, to - 1, arrival };
	}

	std::cout << GetMinTime(teleportations, num_stations, start - 1, end - 1) << '\n';

	system("pause");
	return 0;
}
