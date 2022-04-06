#pragma once


#include "swap_chain.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createSurface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &capabilities);
}