#pragma once

#include "context.hpp"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

using std::vector;

namespace Renderer {
    VkShaderModule createShaderModule(Context *ctx, const vector<char> &code);
}