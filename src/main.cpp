#include <iostream>
#include "swarm_state.hpp"

void swarm_update(SwarmState& state, float dt);

int main() {
    SwarmState state{};
    state.tick = 0;

    // Deterministic agent creation
    for (uint32_t i = 0; i < 6; ++i) {
        state.agents.push_back({
            i,
            float(i * 2),
            0.0f,
            0.0f,
            0.0f
        });
    }

    constexpr float dt = 0.016f;

    for (int step = 0; step < 10; ++step) {
        swarm_update(state, dt);

        std::cout << "tick " << state.tick << "\n";
        for (const auto& a : state.agents) {
            std::cout
                << "  agent " << a.id
                << " pos=(" << a.x << "," << a.y << ")"
                << " vel=(" << a.vx << "," << a.vy << ")\n";
        }
    }

    return 0;
}
