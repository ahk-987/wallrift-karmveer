cmake_minimum_required(VERSION 3.16)

project(wallrift C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(wallrift-daemon
    src/daemon.c
    src/file.c
    src/wayland.c
    src/gl.c
    src/zwlr-layer-shell-protocol.c
    src/xdg-shell-protocol.c
)

target_include_directories(wallrift-daemon PRIVATE include)

target_link_libraries(wallrift-daemon
    wayland-client
    wayland-egl
    wayland-cursor
    EGL
    GLESv2
    m
)

add_executable(wallrift
    src/cli.c
)

target_include_directories(wallrift PRIVATE include)

target_link_libraries(wallrift
    wayland-client
    wayland-egl
    wayland-cursor
    EGL
    GLESv2
    m
)
