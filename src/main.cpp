#include <iostream>
#include <SDL2/SDL.h>

#include "swarm_state.hpp"
#include "swarm_render_gl3.hpp"

void swarm_update(SwarmState& state, float dt);

int main() {
    SwarmState state{};

    if (!swarm_render_gl3_init(800, 600)) {
        std::cerr << "Failed to init GL3 renderer\n";
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        swarm_render_gl3_draw(state);
        SDL_Delay(16);
    }

    swarm_render_gl3_shutdown();
    return 0;
}
