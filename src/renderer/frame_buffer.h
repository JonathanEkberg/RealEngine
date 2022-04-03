#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>
#include "context.h"

namespace Renderer {
    void createFramebuffers(Context *ctx);
}