#pragma once

#include "swarm_state.hpp"

// Initialize window + GL context
bool swarm_render_init(int width, int height);

// Draw current swarm state
void swarm_render_draw(const SwarmState& state);

// Shutdown renderer
void swarm_render_shutdown();
