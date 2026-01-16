#include "swarm_render.hpp"
#include "swarm_neighbors.hpp"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cmath>

enum class Formation { LINE, ORBIT };
static Formation g_formation = Formation::LINE;
static bool g_tilted_view = false;

void swarm_render_set_formation(int f) {
    if (f == 1) g_formation = Formation::LINE;
    if (f == 3) g_formation = Formation::ORBIT;
}

void swarm_render_toggle_perspective() {
    g_tilted_view = !g_tilted_view;
}

static SDL_Window* window = nullptr;
static SDL_GLContext gl_ctx = nullptr;

static constexpr float NEIGHBOR_RADIUS = 3.5f;

/* Grid parameters */
static constexpr float GRID_MIN_X = -10.f;
static constexpr float GRID_MAX_X =  20.f;
static constexpr float GRID_MIN_Y = -10.f;
static constexpr float GRID_MAX_Y =  10.f;
static constexpr float GRID_STEP  =  1.f;
static constexpr int   GRID_MAJOR =  5;

static void setup_camera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Single, stable projection */
    glOrtho(-2, 12, -6, 6, -50, 50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Pull camera back */
    glTranslatef(0.f, 0.f, -12.f);

    /* Rotate downward if enabled */
    if (g_tilted_view) {
        glRotatef(55.f, 1.f, 0.f, 0.f);
    }
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

static void draw_cube(float x, float y, float z, float s) {
    float h = s * 0.5f;
    glBegin(GL_QUADS);

    glVertex3f(x-h,y-h,z+h); glVertex3f(x+h,y-h,z+h);
    glVertex3f(x+h,y+h,z+h); glVertex3f(x-h,y+h,z+h);

    glVertex3f(x-h,y-h,z-h); glVertex3f(x+h,y-h,z-h);
    glVertex3f(x+h,y+h,z-h); glVertex3f(x-h,y+h,z-h);

    glVertex3f(x-h,y+h,z-h); glVertex3f(x+h,y+h,z-h);
    glVertex3f(x+h,y+h,z+h); glVertex3f(x-h,y+h,z+h);

    glVertex3f(x-h,y-h,z-h); glVertex3f(x+h,y-h,z-h);
    glVertex3f(x+h,y-h,z+h); glVertex3f(x-h,y-h,z+h);

    glVertex3f(x-h,y-h,z-h); glVertex3f(x-h,y+h,z-h);
    glVertex3f(x-h,y+h,z+h); glVertex3f(x-h,y-h,z+h);

    glVertex3f(x+h,y-h,z-h); glVertex3f(x+h,y+h,z-h);
    glVertex3f(x+h,y+h,z+h); glVertex3f(x+h,y-h,z+h);

    glEnd();
}

static void draw_grid() {
    glBegin(GL_LINES);
    for (float x = GRID_MIN_X; x <= GRID_MAX_X; x += GRID_STEP) {
        bool major = (int(x) % GRID_MAJOR) == 0;
        if (major) glColor3f(0.25f,0.25f,0.3f);
        else       glColor3f(0.15f,0.15f,0.2f);
        glVertex3f(x, GRID_MIN_Y, 0.f);
        glVertex3f(x, GRID_MAX_Y, 0.f);
    }
    for (float y = GRID_MIN_Y; y <= GRID_MAX_Y; y += GRID_STEP) {
        bool major = (int(y) % GRID_MAJOR) == 0;
        if (major) glColor3f(0.25f,0.25f,0.3f);
        else       glColor3f(0.15f,0.15f,0.2f);
        glVertex3f(GRID_MIN_X, y, 0.f);
        glVertex3f(GRID_MAX_X, y, 0.f);
    }
    glEnd();
}

bool swarm_render_init(int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Sentinel Swarm",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, SDL_WINDOW_OPENGL);
    if (!window) return false;

    gl_ctx = SDL_GL_CreateContext(window);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f,0.05f,0.08f,1.f);
    return true;
}

void swarm_render_draw(const SwarmState& s) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_camera();
    draw_grid();

    glBegin(GL_LINES);
    glColor3f(0.2f,0.6f,0.9f);
    for (uint32_t i = 0; i < s.agents.size(); i++) {
        auto nb = find_neighbors(s, i, NEIGHBOR_RADIUS);
        float zi = visual_height(s,i)*1.5f;
        for (uint32_t j : nb.indices) {
            if (j <= i) continue;
            float zj = visual_height(s,j)*1.5f;
            glVertex3f(s.agents[i].x, s.agents[i].y, zi);
            glVertex3f(s.agents[j].x, s.agents[j].y, zj);
        }
    }
    glEnd();

    for (uint32_t i = 0; i < s.agents.size(); i++) {
        float h = visual_height(s,i);
        float size = 0.3f + h*0.25f;
        glColor3f(h,0.3f*(1.f-h),1.f-h);
        draw_cube(s.agents[i].x, s.agents[i].y, h*1.5f + size*0.5f, size);
    }

    SDL_GL_SwapWindow(window);
}

void swarm_render_shutdown() {
    if (gl_ctx) SDL_GL_DeleteContext(gl_ctx);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
