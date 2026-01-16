#include "swarm_state.hpp"
#include "swarm_neighbors.hpp"
#include <cmath>

static void formation_slot(
    const SwarmState& state,
    uint32_t i,
    float& sx,
    float& sy
) {
    float cx = 0.0f, cy = 0.0f;
    for (const auto& a : state.agents) {
        cx += a.x;
        cy += a.y;
    }
    cx /= state.agents.size();
    cy /= state.agents.size();

    const uint32_t n = state.agents.size();

    switch (state.formation) {
    case FormationMode::LINE: {
        const float spacing = 1.2f;
        const float start = cx - spacing * (n - 1) * 0.5f;
        sx = start + i * spacing;
        sy = cy;
        break;
    }
    case FormationMode::ORBIT: {
        const float r = 2.5f;
        const float t = 2.0f * 3.1415926f * i / n;
        sx = cx + std::cos(t) * r;
        sy = cy + std::sin(t) * r;
        break;
    }
    }
}

void swarm_update(SwarmState& state, float dt) {
    state.tick++;

    // Neighbor interaction forces
    for (uint32_t i = 0; i < state.agents.size(); ++i) {
        auto& a = state.agents[i];
        auto nb = find_neighbors(state, i, 3.5f);

        for (uint32_t j : nb.indices) {
            if (j == i) continue;
            auto& b = state.agents[j];

            float dx = b.x - a.x;
            float dy = b.y - a.y;
            float d2 = dx*dx + dy*dy + 0.001f;

            float force = 0.08f / d2;
            a.vx += dx * force * dt;
            a.vy += dy * force * dt;
        }
    }

    // Formation control
    constexpr float K_FORMATION = 1.2f;
    constexpr float K_DAMPING   = 0.82f;

    for (uint32_t i = 0; i < state.agents.size(); ++i) {
        auto& a = state.agents[i];

        float sx, sy;
        formation_slot(state, i, sx, sy);

        a.vx += (sx - a.x) * K_FORMATION * dt;
        a.vy += (sy - a.y) * K_FORMATION * dt;

        a.vx *= K_DAMPING;
        a.vy *= K_DAMPING;
    }

    // Integrate
    for (auto& a : state.agents) {
        a.x += a.vx * dt;
        a.y += a.vy * dt;
    }
}
