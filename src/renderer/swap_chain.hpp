#pragma once

#include <stdexcept>

#include "context.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;
};

namespace Renderer {
    void createSwapChain(Context *ctx);

    void recreateSwapChain(Context *ctx);

    void cleanupSwapChain(Context *ctx);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    void createImageViews(Renderer::Context *ctx);
}