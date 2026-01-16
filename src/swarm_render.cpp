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

static constexpr float NEIGHBOR_RADIUS = 3.5f;
static constexpr int   CIRCLE_SEGMENTS = 48;

static void setup_camera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 12, -6, 6, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, -1.5f, -4.f);
    glRotatef(35.f, 1.f, 0.f, 0.f);
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
    int n = s.agents.size();

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

static float visual_height(const SwarmState& s, int i) {
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
        glVertex3f(cx + cosf(t)*r, cy + sinf(t)*r, 0.f);
    }
    glEnd();
}

static void draw_agent(float x, float y, float z, float size) {
    glBegin(GL_QUADS);
    glVertex3f(x - size, y - size, z);
    glVertex3f(x + size, y - size, z);
    glVertex3f(x + size, y + size, z);
    glVertex3f(x - size, y + size, z);
    glEnd();
}

bool swarm_render_init(int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        "Sentinel Swarm (3D)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_OPENGL
    );
    if (!window) return false;

    gl_ctx = SDL_GL_CreateContext(window);
    if (!gl_ctx) return false;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.08f, 1.f);
    glLineWidth(1.2f);

    return true;
}

void swarm_render_draw(const SwarmState& s) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_camera();

    // Neighbor radius (ground plane)
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
            glVertex3f(a.x, a.y, 0.f);
            glVertex3f(b.x, b.y, 0.f);
        }
    }
    glEnd();

    // Agents (3D height)
    for (uint32_t i = 0; i < s.agents.size(); i++) {
        float h = visual_height(s, i);
        float z = h * 1.5f;
        float size = 0.1f + h * 0.1f;

        glColor3f(h, 0.3f * (1.f - h), 1.f - h);

        auto& a = s.agents[i];
        draw_agent(a.x, a.y, z, size);
    }

    SDL_GL_SwapWindow(window);
}

void swarm_render_shutdown() {
    if (gl_ctx) SDL_GL_DeleteContext(gl_ctx);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
