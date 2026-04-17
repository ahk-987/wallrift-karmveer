#pragma once

#include "monitor.h"
#include <GL/gl.h>
#include <GLES2/gl2.h>

typedef struct APP APP;
typedef struct GL{
  GLuint prog;
  GLuint vbo;
  GLuint vao;
  GLuint ebo;

  int cursorLoc;
  int imgWLoc;
  int imgHLoc;
  int viewWLoc;
  int viewHLoc;
  int texLoc;

  float speed;
} GL;



GLuint createShader(GLenum type, const char *shaderSrc); 

GLuint createProgram(const char *vFilePath, const char *fFilePath); 
GLuint loadImageIntoGPU(char *imgPath, int *imageWidth, int* imageHeight, GLuint texID); 

void gl_draw(APP *app , Monitor *m);
int setupOpenGL(APP *app);
