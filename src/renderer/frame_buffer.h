#pragma once

#include "context.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

namespace Renderer {
    void createFramebuffers(VkDevice device, VkRenderPass renderPass,
                            std::vector<VkFramebuffer> *swapChainFramebuffers, VkExtent2D swapChainExtent,
                            std::vector<VkImageView> swapChainImageViews);
}