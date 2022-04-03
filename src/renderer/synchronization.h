#pragma once

#include "context.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Renderer {
    void createSyncObjects(Context *ctx);
}
