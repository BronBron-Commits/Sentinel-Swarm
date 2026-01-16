#pragma once

#include "swarm_state.hpp"

bool swarm_render_gl3_init(int width, int height);
void swarm_render_gl3_draw(const SwarmState& state);
void swarm_render_gl3_shutdown();
