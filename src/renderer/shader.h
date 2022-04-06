#pragma once


#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>

using std::vector;

namespace Renderer {
    VkShaderModule createShaderModule(VkDevice device, const vector<char> &code);
}