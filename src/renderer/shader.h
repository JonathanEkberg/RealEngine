#pragma once

#include "context.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>

using std::vector;

namespace Renderer {
    VkShaderModule createShaderModule(Context *ctx, const vector<char> &code);
}