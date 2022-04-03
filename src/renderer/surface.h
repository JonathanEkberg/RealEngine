#pragma once

#include "context.h"
#include "swap_chain.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createSurface(Context *ctx);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(Context *ctx, const VkSurfaceCapabilitiesKHR &capabilities);
}