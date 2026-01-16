#include "swarm_render.hpp"
#include "swarm_neighbors.hpp"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cmath>

enum class Formation { LINE, ORBIT };
static Formation g_formation = Formation::LINE;

void swarm_render_set_formation(int f) {
    if (f == 1) g_formation = Formation::LINE;
    if (f == 3) g_formation = Formation::ORBIT;
}

static SDL_Window* window = nullptr;
static SDL_GLContext gl_ctx = nullptr;

static constexpr float WORLD_MIN_X = -2.f;
static constexpr float WORLD_MAX_X = 12.f;
static constexpr float WORLD_MIN_Y = -6.f;
static constexpr float WORLD_MAX_Y = 6.f;

static constexpr float NEIGHBOR_RADIUS = 3.5f;
static constexpr int   CIRCLE_SEGMENTS = 48;

static float nx(float x) {
    return (x - WORLD_MIN_X) / (WORLD_MAX_X - WORLD_MIN_X) * 2.f - 1.f;
}
static float ny(float y) {
    return (y - WORLD_MIN_Y) / (WORLD_MAX_Y - WORLD_MIN_Y) * 2.f - 1.f;
}

static void center_of_mass(const SwarmState& s, float& cx, float& cy) {
    cx = cy = 0.f;
    for (auto& a : s.agents) {
        cx += a.x;
        cy += a.y;
    }
    cx /= s.agents.size();
    cy /= s.agents.size();
}

static void formation_slot(const SwarmState& s, int i, float& sx, float& sy) {
    float cx, cy;
    center_of_mass(s, cx, cy);
    const int n = s.agents.size();

    if (g_formation == Formation::LINE) {
        float spacing = 1.2f;
        float start = cx - spacing * (n - 1) * 0.5f;
        sx = start + i * spacing;
        sy = cy;
    } else {
        float r = 2.5f;
        float t = 2.f * 3.1415926f * i / n;
        sx = cx + cosf(t) * r;
        sy = cy + sinf(t) * r;
    }
}

static float compute_visual_height(const SwarmState& s, int i) {
    float sx, sy;
    formation_slot(s, i, sx, sy);
    auto& a = s.agents[i];
    float dx = a.x - sx;
    float dy = a.y - sy;
    float err = sqrtf(dx*dx + dy*dy);
    return fminf(err / 2.5f, 1.f);
}

static void draw_circle(float cx, float cy, float r) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < CIRCLE_SEGMENTS; i++) {
        float t = 2.f * 3.1415926f * i / CIRCLE_SEGMENTS;
        glVertex2f(nx(cx + cosf(t)*r), ny(cy + sinf(t)*r));
    }
    glEnd();
}

static void draw_agent_quad(float x, float y, float size) {
    glBegin(GL_QUADS);
    glVertex2f(nx(x - size), ny(y - size));
    glVertex2f(nx(x + size), ny(y - size));
    glVertex2f(nx(x + size), ny(y + size));
    glVertex2f(nx(x - size), ny(y + size));
    glEnd();
}

bool swarm_render_init(int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_CreateWindow(
        "Sentinel Swarm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_OPENGL
    );
    if (!window) return false;

    gl_ctx = SDL_GL_CreateContext(window);
    if (!gl_ctx) return false;

    glClearColor(0.05f, 0.05f, 0.08f, 1.f);
    glLineWidth(1.2f);
    return true;
}

void swarm_render_draw(const SwarmState& s) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Neighbor radius
    glColor3f(0.25f, 0.35f, 0.45f);
    for (auto& a : s.agents)
        draw_circle(a.x, a.y, NEIGHBOR_RADIUS);

    // Neighbor links
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.5f, 0.7f);
    for (uint32_t i = 0; i < s.agents.size(); i++) {
        auto nb = find_neighbors(s, i, NEIGHBOR_RADIUS);
        auto& a = s.agents[i];
        for (uint32_t j : nb.indices) {
            if (j <= i) continue;
            auto& b = s.agents[j];
            glVertex2f(nx(a.x), ny(a.y));
            glVertex2f(nx(b.x), ny(b.y));
        }
    }
    glEnd();

    // Agents (height-encoded quads)
    for (uint32_t i = 0; i < s.agents.size(); i++) {
        float h = compute_visual_height(s, i);

        float size = 0.08f + h * 0.08f;
        float lift = h * 0.25f;

        glColor3f(h, 0.3f * (1.f - h), 1.f - h);

        auto& a = s.agents[i];
        draw_agent_quad(a.x, a.y + lift, size);
    }

    SDL_GL_SwapWindow(window);
}

void swarm_render_shutdown() {
    if (gl_ctx) SDL_GL_DeleteContext(gl_ctx);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
