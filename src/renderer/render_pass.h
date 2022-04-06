#pragma once


#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createRenderPass(VkDevice device, VkFormat swapChainImageFormat, VkRenderPass &renderPass);
}