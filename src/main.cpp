#include <iostream>
#include <SDL2/SDL.h>

#include "swarm_state.hpp"
#include "swarm_render.hpp"
#include "swarm_update.hpp"

int main() {
    SwarmState state{};

    for (uint32_t i = 0; i < 6; ++i) {
        state.agents.push_back({
            i,
            float(i * 2),
            0.0f,
            0.0f,
            0.0f
        });
    }

    if (!swarm_render_init(800, 600)) {
        std::cerr << "Failed to init renderer\n";
        return 1;
    }

    constexpr float dt = 0.016f;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_1)
                    state.formation = FormationMode::LINE;

                if (e.key.keysym.sym == SDLK_3)
                    state.formation = FormationMode::ORBIT;

                if (e.key.keysym.sym == SDLK_p)
                    swarm_render_toggle_perspective();
            }
        }

        swarm_update(state, dt);
        swarm_render_draw(state);
        SDL_Delay(16);
    }

    swarm_render_shutdown();
    return 0;
}
