#include "swap_chain.h"
#include "render_pass.h"
#include "pipeline.h"
#include "buffer/frame_buffer.h"
#include "surface.h"
#include "device.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

void Renderer::createSwapChain(CreateSwapChainInfo &info) {
    SwapChainSupportDetails swapChainSupport = Renderer::querySwapChainSupport(info.physicalDevice, info.surface);

    VkSurfaceFormatKHR surfaceFormat = Renderer::chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = Renderer::chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = Renderer::chooseSwapExtent(info.window, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = info.surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(info.physicalDevice, info.surface);
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

    if (vkCreateSwapchainKHR(info.device, &createInfo, nullptr, &info.pSwapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(info.device, info.pSwapChain, &imageCount, nullptr);
    info.swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(info.device, info.pSwapChain, &imageCount, info.swapChainImages.data());

    info.swapChainImageFormat = surfaceFormat.format;
    info.swapChainExtent = extent;

    std::cout << "Successfully created swap chain!" << std::endl;
}

void Renderer::createImageViews(VkDevice device, std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat,
                                std::vector<VkImageView> *swapChainImageViews) {
//    if (!swapChainImageViews->empty()) {
//        for (auto imageView: *swapChainImageViews) {
//            vkDestroyImageView(device, imageView, nullptr);
//        }
//    }

    swapChainImageViews->resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews->at(i)) != VK_SUCCESS) {
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

void Renderer::recreateSwapChain(RecreateSwapChainData &data) {
    auto createData = data.createInfo;

    int width = 0, height = 0;
    glfwGetFramebufferSize(createData.window, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(createData.window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(createData.device);

    Renderer::cleanupSwapChain(data.createInfo.device, data.pipeline, data.pipelineLayout, data.pRenderPass,
                               data.createInfo.pSwapChain, data.swapChainImageViews, data.swapChainFramebuffers);

    Renderer::createSwapChain(createData);
    Renderer::createImageViews(createData.device, createData.swapChainImages, createData.swapChainImageFormat,
                               &data.swapChainImageViews);
    Renderer::createRenderPass(createData.device, createData.swapChainImageFormat, data.pRenderPass);
    Renderer::createGraphicsPipeline(createData.device, data.pRenderPass,
                                     createData.swapChainExtent, data.pipeline, data.pipelineLayout);
    Renderer::createFramebuffers(createData.device, data.pRenderPass, &data.swapChainFramebuffers,
                                 createData.swapChainExtent, data.swapChainImageViews);
}

void Renderer::cleanupSwapChain(VkDevice &device, VkPipeline &graphicsPipeline, VkPipelineLayout &pipelineLayout,
                                VkRenderPass &renderPass, VkSwapchainKHR &swapChain,
                                const std::vector<VkImageView> &swapChainImageViews,
                                const std::vector<VkFramebuffer> &swapChainFramebuffers) {
    for (auto framebuffer: swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    vkDestroyRenderPass(device, renderPass, nullptr);
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

    for (auto imageView: swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
}
