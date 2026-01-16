#include "swarm_render.hpp"
#include "swarm_neighbors.hpp"

#include <SDL2/SDL.h>
#include <GL/gl.h>

static SDL_Window* window = nullptr;
static SDL_GLContext gl_ctx = nullptr;

static int win_w = 800;
static int win_h = 600;

// World bounds for visualization
static constexpr float WORLD_MIN_X = -2.0f;
static constexpr float WORLD_MAX_X = 12.0f;
static constexpr float WORLD_MIN_Y = -6.0f;
static constexpr float WORLD_MAX_Y = 6.0f;

// Must match simulation
static constexpr float NEIGHBOR_RADIUS = 3.5f;

static float norm_x(float x) {
    return (x - WORLD_MIN_X) / (WORLD_MAX_X - WORLD_MIN_X) * 2.0f - 1.0f;
}

static float norm_y(float y) {
    return (y - WORLD_MIN_Y) / (WORLD_MAX_Y - WORLD_MIN_Y) * 2.0f - 1.0f;
}

bool swarm_render_init(int width, int height) {
    win_w = width;
    win_h = height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return false;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "Sentinel Swarm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        win_w,
        win_h,
        SDL_WINDOW_OPENGL
    );

    if (!window)
        return false;

    gl_ctx = SDL_GL_CreateContext(window);
    if (!gl_ctx)
        return false;

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glPointSize(8.0f);
    glLineWidth(1.5f);

    return true;
}

void swarm_render_draw(const SwarmState& state) {
    glClear(GL_COLOR_BUFFER_BIT);

    // =========================
    // Neighbor Links
    // =========================
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.5f, 0.7f);

    for (uint32_t i = 0; i < state.agents.size(); ++i) {
        const auto neighbors = find_neighbors(state, i, NEIGHBOR_RADIUS);
        const auto& a = state.agents[i];

        for (uint32_t n : neighbors.indices) {
            // Draw each link once (avoid double-draw)
            if (n <= i)
                continue;

            const auto& b = state.agents[n];

            glVertex2f(norm_x(a.x), norm_y(a.y));
            glVertex2f(norm_x(b.x), norm_y(b.y));
        }
    }
    glEnd();

    // =========================
    // Agents
    // =========================
    glBegin(GL_POINTS);
    for (const auto& agent : state.agents) {
        glColor3f(0.3f, 0.8f, 1.0f);
        glVertex2f(
            norm_x(agent.x),
            norm_y(agent.y)
        );
    }
    glEnd();

    SDL_GL_SwapWindow(window);
}

void swarm_render_shutdown() {
    if (gl_ctx) {
        SDL_GL_DeleteContext(gl_ctx);
        gl_ctx = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}
