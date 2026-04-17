#pragma once

#include "wayland.h"
#include "egl.h"
#include "gl.h"
#include "monitor.h"
#define MAX_MONITORS 5

typedef struct APP {

    WLGlobal wl;
    EGLState egl;
    GL gl;

    Monitor monitors[MAX_MONITORS];
    int monitor_count;

    Monitor *active_monitor;
    
} APP;

  

