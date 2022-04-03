#pragma once

#include "context.hpp"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Renderer {
    void createSyncObjects(Context *ctx);
}
