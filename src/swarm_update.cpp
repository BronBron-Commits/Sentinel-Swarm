#include "swarm_update.hpp"
#include "swarm_neighbors.hpp"
#include <cmath>

static constexpr float NEIGHBOR_RADIUS = 3.0f;
static constexpr float K_FORM = 1.5f;
static constexpr float K_DAMP = 0.9f;
static constexpr float MAX_V = 5.0f;

void swarm_update(SwarmState& state, float dt) {
    std::vector<std::vector<uint32_t>> neighbors;
    compute_neighbors(state, NEIGHBOR_RADIUS, neighbors);

    const size_t n = state.agents.size();

    for (size_t i = 0; i < n; ++i) {
        auto& a = state.agents[i];

        float tx = a.x;
        float ty = a.y;

        if (state.formation == FormationMode::LINE) {
            tx = float(a.id) * 2.0f;
            ty = 0.0f;
        }
        else if (state.formation == FormationMode::ORBIT) {
            float angle = float(a.id) / float(n) * 6.2831853f;
            tx = std::cos(angle) * 5.0f;
            ty = std::sin(angle) * 5.0f;
        }

        float ex = tx - a.x;
        float ey = ty - a.y;

        a.error_mag = std::sqrt(ex * ex + ey * ey);

        a.vx = a.vx * K_DAMP + ex * K_FORM * dt;
        a.vy = a.vy * K_DAMP + ey * K_FORM * dt;

        float v2 = a.vx * a.vx + a.vy * a.vy;
        if (v2 > MAX_V * MAX_V) {
            float inv = MAX_V / std::sqrt(v2);
            a.vx *= inv;
            a.vy *= inv;
        }
    }

    for (auto& a : state.agents) {
        a.x += a.vx * dt;
        a.y += a.vy * dt;
    }

    state.tick++;
}
