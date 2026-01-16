#pragma once

#include <vector>
#include <cstdint>

struct SwarmAgent {
    uint32_t id;
    float x;
    float y;
    float vx;
    float vy;
};

struct SwarmState {
    uint64_t tick;
    std::vector<SwarmAgent> agents;
};
