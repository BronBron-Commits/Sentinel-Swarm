#pragma once
#include <vector>
#include <cstdint>

enum class FormationMode {
    LINE,
    ORBIT
};

struct Agent {
    uint32_t id;
    float x, y;
    float vx, vy;

    // diagnostics
    uint32_t neighbor_count = 0;
    float error_mag = 0.0f;
};

struct SwarmState {
    uint64_t tick = 0;
    FormationMode formation = FormationMode::LINE;
    std::vector<Agent> agents;
};
