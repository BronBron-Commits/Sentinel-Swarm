#include "swarm_state.hpp"
#include "swarm_neighbors.hpp"

constexpr float NEIGHBOR_RADIUS = 3.5f;
constexpr float COHESION_STRENGTH = 0.25f;

void swarm_update(SwarmState& state, float dt) {
    // Temporary velocity deltas (deterministic size)
    std::vector<float> dvx(state.agents.size(), 0.0f);
    std::vector<float> dvy(state.agents.size(), 0.0f);

    for (uint32_t i = 0; i < state.agents.size(); ++i) {
        const auto neighbors = find_neighbors(state, i, NEIGHBOR_RADIUS);

        if (neighbors.indices.empty())
            continue;

        float avg_x = 0.0f;
        float avg_y = 0.0f;

        for (uint32_t n : neighbors.indices) {
            avg_x += state.agents[n].x;
            avg_y += state.agents[n].y;
        }

        avg_x /= neighbors.indices.size();
        avg_y /= neighbors.indices.size();

        const float dx = avg_x - state.agents[i].x;
        const float dy = avg_y - state.agents[i].y;

        dvx[i] = dx * COHESION_STRENGTH;
        dvy[i] = dy * COHESION_STRENGTH;
    }

    // Apply updates in a second pass (critical for determinism)
    for (uint32_t i = 0; i < state.agents.size(); ++i) {
        auto& a = state.agents[i];

        a.vx += dvx[i];
        a.vy += dvy[i];

        a.x += a.vx * dt;
        a.y += a.vy * dt;
    }

    state.tick++;
}
