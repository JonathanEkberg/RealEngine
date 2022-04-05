#pragma once

#include "context.h"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;
};

namespace Renderer {
    struct CreateSwapChainInfo {
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkSurfaceKHR surface;
        GLFWwindow *window;
        VkSwapchainKHR &pSwapChain;
        std::vector<VkImage> &pSwapChainImages;
        VkFormat &pSwapChainImageFormat;
        VkExtent2D &pSwapChainExtent;
    };

    void createSwapChain(CreateSwapChainInfo &info);

    struct RecreateSwapChainData {
        CreateSwapChainInfo createInfo;

        VkRenderPass renderPass;
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkImageView> &pSwapChainImageViews;
        std::vector<VkFramebuffer> &swapChainFramebuffers;
    };

    void recreateSwapChain(RecreateSwapChainData &data);

    void cleanupSwapChain(Context &ctx);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    void createImageViews(VkDevice device, std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat,
                          std::vector<VkImageView> *swapChainImageViews);
}