#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace Renderer {
    void createFramebuffers(VkDevice device, VkRenderPass renderPass,
                            std::vector<VkFramebuffer> *swapChainFramebuffers, VkExtent2D swapChainExtent,
                            std::vector<VkImageView> swapChainImageViews);
}