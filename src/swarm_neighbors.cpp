#include "swarm_neighbors.hpp"
#include <cmath>

void compute_neighbors(
    SwarmState& state,
    float radius,
    std::vector<std::vector<uint32_t>>& out_neighbors
) {
    const float r2 = radius * radius;
    const size_t n = state.agents.size();

    out_neighbors.clear();
    out_neighbors.resize(n);

    for (size_t i = 0; i < n; ++i) {
        auto& a = state.agents[i];
        a.neighbor_count = 0;

        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;

            auto& b = state.agents[j];
            float dx = b.x - a.x;
            float dy = b.y - a.y;

            if (dx * dx + dy * dy <= r2) {
                out_neighbors[i].push_back(b.id);
                a.neighbor_count++;
            }
        }
    }
}
