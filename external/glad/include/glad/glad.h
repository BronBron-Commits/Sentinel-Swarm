#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef int GLint;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef float GLfloat;

typedef void* (*GLADloadproc)(const char* name);

int gladLoadGLLoader(GLADloadproc load);

/* --- Minimal constants used --- */
#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS  0x8B81
#define GL_LINK_STATUS     0x8B82
#define GL_ARRAY_BUFFER    0x8892
#define GL_STATIC_DRAW     0x88E4
#define GL_FLOAT           0x1406
#define GL_FALSE           0
#define GL_TRIANGLES       0x0004
#define GL_COLOR_BUFFER_BIT 0x4000

/* --- Function declarations (resolved via libGL) --- */
GLuint glCreateShader(GLenum);
void glShaderSource(GLuint, GLsizei, const char* const*, const GLint*);
void glCompileShader(GLuint);
void glGetShaderiv(GLuint, GLenum, GLint*);
void glGetShaderInfoLog(GLuint, GLsizei, GLsizei*, char*);
GLuint glCreateProgram(void);
void glAttachShader(GLuint, GLuint);
void glLinkProgram(GLuint);
void glDeleteShader(GLuint);
void glUseProgram(GLuint);
void glDeleteProgram(GLuint);

void glGenVertexArrays(GLsizei, GLuint*);
void glBindVertexArray(GLuint);
void glGenBuffers(GLsizei, GLuint*);
void glBindBuffer(GLenum, GLuint);
void glBufferData(GLenum, intptr_t, const void*, GLenum);
void glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
void glEnableVertexAttribArray(GLuint);
void glDrawArrays(GLenum, GLint, GLsizei);
void glClearColor(GLfloat, GLfloat, GLfloat, GLfloat);
void glClear(GLbitfield);
void glDeleteBuffers(GLsizei, const GLuint*);
void glDeleteVertexArrays(GLsizei, const GLuint*);

#ifdef __cplusplus
}
#endif
