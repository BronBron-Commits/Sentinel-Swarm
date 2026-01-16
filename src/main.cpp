#include <iostream>
#include "swarm_state.hpp"

void swarm_update(SwarmState& state, float dt);

int main() {
    SwarmState state{};
    state.tick = 0;

    for (uint32_t i = 0; i < 5; ++i) {
        state.agents.push_back({
            i,
            float(i * 2),
            0.0f,
            1.0f,
            0.0f
        });
    }

    constexpr float dt = 0.016f;

    for (int i = 0; i < 10; ++i) {
        swarm_update(state, dt);
        std::cout << "tick " << state.tick << "\n";
    }

    return 0;
}
