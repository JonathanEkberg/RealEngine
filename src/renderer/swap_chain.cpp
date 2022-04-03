#include "swap_chain.hpp"
#include "render_pass.hpp"
#include "pipeline.hpp"
#include "frame_buffer.hpp"

void Renderer::createImageViews(Renderer::Context *ctx) {
    ctx->swapChainImageViews.resize(ctx->swapChainImages.size());

    for (size_t i = 0; i < ctx->swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = ctx->swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = ctx->swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(ctx->device, &createInfo, nullptr, &ctx->swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}


SwapChainSupportDetails Renderer::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                                  details.presentModes.data());
    }

    return details;
}

void Renderer::recreateSwapChain(Renderer::Context *ctx) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(ctx->window, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(ctx->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(ctx->device);

    Renderer::createSwapChain(ctx);
    Renderer::createImageViews(ctx);
    Renderer::createRenderPass(ctx);
    Renderer::createGraphicsPipeline(ctx);
    Renderer::createFramebuffers(ctx);
}

void Renderer::cleanupSwapChain(Renderer::Context *ctx) {
    for (auto framebuffer: ctx->swapChainFramebuffers) {
        vkDestroyFramebuffer(ctx->device, framebuffer, nullptr);
    }

    vkDestroyRenderPass(ctx->device, ctx->renderPass, nullptr);
    vkDestroyPipeline(ctx->device, ctx->graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(ctx->device, ctx->pipelineLayout, nullptr);

    for (auto imageView: ctx->swapChainImageViews) {
        vkDestroyImageView(ctx->device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(ctx->device, ctx->swapChain, nullptr);
}
