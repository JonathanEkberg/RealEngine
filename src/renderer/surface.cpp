#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "surface.hpp"
#include "context.hpp"
#include "device.hpp"

#include <stdexcept>
#include <iostream>
#include <algorithm>

void Renderer::createSurface(Context *ctx) {
    if (glfwCreateWindowSurface(ctx->instance, ctx->window, nullptr, &ctx->surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }

    std::cout << "Successfully created window surface!" << std::endl;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats) {
    // Check for SRGB support
    for (const auto &availableFormats: availableFormats) {
        if (availableFormats.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormats.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormats;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes) {
    for (const auto &availablePresentMode: availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(Context *ctx, const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(ctx->window, &width, &height);

    VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                    capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                     capabilities.maxImageExtent.height);

    return actualExtent;
}


void Renderer::createSwapChain(Renderer::Context *ctx) {
    SwapChainSupportDetails swapChainSupport = Renderer::querySwapChainSupport(ctx->physicalDevice, ctx->surface);

    VkSurfaceFormatKHR surfaceFormat = Renderer::chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = Renderer::chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = Renderer::chooseSwapExtent(ctx, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = ctx->surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(ctx->physicalDevice, ctx->surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(ctx->device, &createInfo, nullptr, &ctx->swapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(ctx->device, ctx->swapChain, &imageCount, nullptr);
    ctx->swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(ctx->device, ctx->swapChain, &imageCount, ctx->swapChainImages.data());

    ctx->swapChainImageFormat = surfaceFormat.format;
    ctx->swapChainExtent = extent;

    std::cout << "Successfully created swap chain!" << std::endl;
}


