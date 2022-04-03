#pragma once

#include "context.hpp"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createInstance(Context *ctx);
}
