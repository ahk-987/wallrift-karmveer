#pragma once

#include <EGL/egl.h>

typedef struct EGLState {

    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLConfig egl_config;

} EGLState;
