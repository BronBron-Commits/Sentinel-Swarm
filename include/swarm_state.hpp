#pragma once

#include <vector>
#include <cstdint>

struct SwarmAgent {
    uint32_t id;

    // position
    float x;
    float y;

    // velocity
    float vx;
    float vy;
};

struct SwarmState {
    uint64_t tick;

    // IMPORTANT:
    // agents vector order is deterministic and stable
    // (sorted by id, never reordered during simulation)
    std::vector<SwarmAgent> agents;
};
