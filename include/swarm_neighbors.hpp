#pragma once

#include "swarm_state.hpp"
#include <vector>
#include <cstdint>

// Result is a list of agent indices, not pointers.
// Indices are deterministic and stable.
struct NeighborList {
    std::vector<uint32_t> indices;
};

// Deterministic neighbor query:
// - O(N^2), explicit and predictable
// - No spatial hashing yet (by design)
NeighborList find_neighbors(
    const SwarmState& state,
    uint32_t self_index,
    float radius
);
