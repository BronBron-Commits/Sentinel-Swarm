#include "swarm_neighbors.hpp"

static inline float sqr(float v) {
    return v * v;
}

NeighborList find_neighbors(
    const SwarmState& state,
    uint32_t self_index,
    float radius
) {
    NeighborList result;
    const float r2 = radius * radius;

    const SwarmAgent& self = state.agents[self_index];

    // Deterministic iteration:
    // - forward index order
    // - skip self explicitly
    for (uint32_t i = 0; i < state.agents.size(); ++i) {
        if (i == self_index)
            continue;

        const SwarmAgent& other = state.agents[i];

        const float dx = other.x - self.x;
        const float dy = other.y - self.y;

        if ((dx * dx + dy * dy) <= r2) {
            result.indices.push_back(i);
        }
    }

    return result;
}
