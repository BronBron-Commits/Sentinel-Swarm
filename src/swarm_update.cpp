#include "swarm_state.hpp"

void swarm_update(SwarmState& state, float dt) {
    for (auto& agent : state.agents) {
        agent.x += agent.vx * dt;
        agent.y += agent.vy * dt;
    }

    state.tick++;
}
