#pragma once
#include "swarm_state.hpp"
#include <vector>

void compute_neighbors(
    SwarmState& state,
    float radius,
    std::vector<std::vector<uint32_t>>& out_neighbors
);
