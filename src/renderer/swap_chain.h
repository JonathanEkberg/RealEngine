#pragma once


#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace Renderer {
    struct CreateSwapChainInfo {
        VkPhysicalDevice &physicalDevice;
        VkDevice &device;
        VkSurfaceKHR &surface;
        GLFWwindow *window;
        VkSwapchainKHR &pSwapChain;
        std::vector<VkImage> &swapChainImages;
        VkFormat &swapChainImageFormat;
        VkExtent2D &swapChainExtent;
    };

    void createSwapChain(CreateSwapChainInfo &info);

    struct RecreateSwapChainData {
        CreateSwapChainInfo createInfo;

        VkPipeline &pipeline;
        VkPipelineLayout &pipelineLayout;
        VkRenderPass &pRenderPass;
        std::vector<VkImageView> &swapChainImageViews;
        std::vector<VkFramebuffer> &swapChainFramebuffers;
    };

    void recreateSwapChain(RecreateSwapChainData &data);

    void cleanupSwapChain(VkDevice &device, VkPipeline &graphicsPipeline, VkPipelineLayout &pipelineLayout,
                          VkRenderPass &renderPass, VkSwapchainKHR &swapChain,
                          const std::vector<VkImageView> &swapChainImageViews,
                          const std::vector<VkFramebuffer> &swapChainFramebuffers);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    void createImageViews(VkDevice device, std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat,
                          std::vector<VkImageView> *swapChainImageViews);
}