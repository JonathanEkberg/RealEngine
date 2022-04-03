#pragma once

#include "context.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createFramebuffers(Context *ctx);
}