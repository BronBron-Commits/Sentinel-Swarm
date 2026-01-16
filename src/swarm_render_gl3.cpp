#include <SDL2/SDL.h>

/* Enable modern OpenGL prototypes */
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <iostream>
#include <cmath>

#include "swarm_state.hpp"
#include "swarm_render.hpp"

/* =========================
   GL state
   ========================= */
static SDL_Window* window = nullptr;
static SDL_GLContext gl_ctx = nullptr;
static GLuint prog = 0;
static GLuint vao = 0;
static GLuint vbo = 0;

/* =========================
   Shaders
   ========================= */
static const char* vs_src = R"(
#version 330 core
layout (location = 0) in vec3 pos;
uniform mat4 MVP;
void main() {
    gl_Position = MVP * vec4(pos, 1.0);
}
)";

static const char* fs_src = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.3, 0.8, 1.0, 1.0);
}
)";

/* =========================
   Cube mesh
   ========================= */
static float cube[] = {
    -0.5,-0.5, 0.5,  0.5,-0.5, 0.5,  0.5, 0.5, 0.5,
     0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5,-0.5, 0.5,
    -0.5,-0.5,-0.5, -0.5, 0.5,-0.5,  0.5, 0.5,-0.5,
     0.5, 0.5,-0.5,  0.5,-0.5,-0.5, -0.5,-0.5,-0.5,
};

/* =========================
   Helpers
   ========================= */
static GLuint compile(GLenum t, const char* src) {
    GLuint s = glCreateShader(t);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

static void make_mvp(float* m, float x, float y, float z) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

bool swarm_render_init(int w, int h) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        "Sentinel Swarm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_OPENGL
    );

    gl_ctx = SDL_GL_CreateContext(window);

    GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);

    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    return true;
}

void swarm_render_draw(const SwarmState& state) {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(prog);
    glBindVertexArray(vao);

    for (const auto& a : state.agents) {
        float mvp[16];
        make_mvp(mvp, a.x, 0.0f, a.y);

        glUniformMatrix4fv(
            glGetUniformLocation(prog, "MVP"),
            1, GL_FALSE, mvp
        );

        glDrawArrays(GL_TRIANGLES, 0, 12);
    }

    SDL_GL_SwapWindow(window);
}

void swarm_render_shutdown() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(prog);
    SDL_GL_DeleteContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void swarm_render_toggle_perspective() {
    /* placeholder for now — keeps API stable */
}
