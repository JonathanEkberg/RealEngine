#pragma once

#include <optional>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

using std::optional;
using std::vector;

class VulkanApplication {
public:
    void run();

private:
    static constexpr size_t WIDTH = 1920;
    static constexpr size_t HEIGHT = 1080;

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
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    vector<VkSemaphore> imageAvailableSemaphores, renderFinishedSemaphores;
    vector<VkFence> inFlightFences;

    // Vertex data
    VkBuffer vertexBuffer{};
    VkDeviceMemory vertexBufferMemory;

    // Index data
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    uint32_t currentFrame = 0;
    bool framebufferResized = false;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void drawFrame();

    void cleanup();
};

