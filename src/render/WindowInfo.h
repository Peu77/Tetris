//
// Created by peu77 on 9/10/22.
//

#pragma once

#include "GLFW/glfw3.h"

struct WindowInfo {
    int width;
    int height;
    const char *title;
    GLFWwindow *window;
};