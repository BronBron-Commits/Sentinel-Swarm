#pragma once

#include "swarm_state.hpp"

bool swarm_render_init(int width, int height);
void swarm_render_draw(const SwarmState& state);
void swarm_render_shutdown();

// Formation overlay control (render-only)
void swarm_render_set_formation(int f);
