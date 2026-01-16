#include "swarm_render_gl3.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <cstdio>

static SDL_Window* window = nullptr;
static SDL_GLContext ctx = nullptr;

static GLuint prog = 0;
static GLuint vao = 0;
static GLuint vbo = 0;

static GLuint compile(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);

    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(s, 512, nullptr, log);
        fprintf(stderr, "shader error: %s\n", log);
    }
    return s;
}

bool swarm_render_gl3_init(int w, int h) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        "Sentinel Swarm (GL3)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_OPENGL
    );
    if (!window) return false;

    ctx = SDL_GL_CreateContext(window);
    if (!ctx) return false;

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        fprintf(stderr, "Failed to init GLAD\n");
        return false;
    }

    const char* vs_src = R"(
        #version 330 core
        layout (location = 0) in vec2 pos;
        void main() {
            gl_Position = vec4(pos, 0.0, 1.0);
        }
    )";

    const char* fs_src = R"(
        #version 330 core
        out vec4 color;
        void main() {
            color = vec4(0.2, 0.8, 1.0, 1.0);
        }
    )";

    GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);

    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    float verts[] = {
         0.0f,  0.6f,
        -0.6f, -0.6f,
         0.6f, -0.6f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return true;
}

void swarm_render_gl3_draw(const SwarmState&) {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);
}

void swarm_render_gl3_shutdown() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(prog);

    if (ctx) SDL_GL_DeleteContext(ctx);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
