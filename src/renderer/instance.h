#pragma once


#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createInstance(VkInstance *instance);
}
