#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>

using std::vector;

namespace Renderer {
    struct Context {
        uint32_t currentFrame = 0;

        GLFWwindow *window;

        VkInstance instance;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice;
        VkDevice device;

        VkQueue graphicsQueue, presentQueue;

        VkSwapchainKHR swapChain;
        vector<VkImage> swapChainImages;
        vector<VkImageView> swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;
        vector<VkCommandBuffer> commandBuffers;

        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        vector<VkSemaphore> imageAvailableSemaphores, renderFinishedSemaphores;
        vector<VkFence> inFlightFences;
    };

}

