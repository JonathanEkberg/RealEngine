#include "game.h"
#include "renderer/instance.h"
#include "renderer/surface.h"
#include "renderer/device.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/buffer/frame_buffer.h"
#include "renderer/command_pool.h"
#include "renderer/buffer/vertex_buffer.h"
#include "renderer/buffer/index_buffer.h"
#include "renderer/buffer/command_buffer.h"
#include "renderer/synchronization.h"
#include "renderer/constants.h"
#include "renderer/buffer/uniform_buffer.h"

#include <iostream>

void VulkanApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanApplication::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    std::cout << "Vulkan supported?: " << glfwVulkanSupported() << std::endl;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<VulkanApplication *>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    });
}

void VulkanApplication::initVulkan() {
    Renderer::createInstance(&instance);
    Renderer::createSurface(window, instance, &surface);
    Renderer::pickPhysicalDevice(instance, surface, &physicalDevice);
    Renderer::createLogicalDevice(physicalDevice, surface, &device, &graphicsQueue, &presentQueue);

    Renderer::CreateSwapChainInfo createSwapChainData{
            physicalDevice,
            device,
            surface,
            window,
            swapChain,
            swapChainImages,
            swapChainImageFormat,
            swapChainExtent
    };
    Renderer::createSwapChain(createSwapChainData);

    Renderer::createImageViews(device, swapChainImages, swapChainImageFormat, &swapChainImageViews);
    Renderer::createRenderPass(device, swapChainImageFormat, renderPass);
    Renderer::createDescriptorSetLayout();
    Renderer::createGraphicsPipeline(device, renderPass, swapChainExtent,
                                     graphicsPipeline, pipelineLayout);
    Renderer::createFramebuffers(device, renderPass, &swapChainFramebuffers, swapChainExtent,
                                 swapChainImageViews);
    Renderer::createCommandPool(device, physicalDevice, surface, &commandPool);

    Renderer::CreateVertexBufferInfo createVertexBufferInfo{
            device,
            physicalDevice,
            commandPool,
            graphicsQueue,
            vertexBuffer,
            vertexBufferMemory,
    };
    Renderer::createVertexBuffer(createVertexBufferInfo);

    Renderer::createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue, &indexBuffer,
                                &indexBufferMemory);
    Renderer::createCommandBuffers(device, commandPool, commandBuffers);
    Renderer::createSyncObjects(device, imageAvailableSemaphores, renderFinishedSemaphores,
                                inFlightFences);
}

void VulkanApplication::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void VulkanApplication::drawFrame() {
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,
                                            imageAvailableSemaphores[currentFrame],
                                            VK_NULL_HANDLE,
                                            &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        Renderer::RecreateSwapChainData recreateSwapChainData{
                Renderer::CreateSwapChainInfo{
                        physicalDevice,
                        device,
                        surface,
                        window,
                        swapChain,
                        swapChainImages,
                        swapChainImageFormat,
                        swapChainExtent,
                },
                graphicsPipeline,
                pipelineLayout,
                renderPass,
                swapChainImageViews,
                swapChainFramebuffers,
        };
        Renderer::recreateSwapChain(recreateSwapChainData);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to aquire swap chain image!");
    }

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    Renderer::RecordCommandBufferInfo recordCommandBufferInfo{
            imageIndex,
            graphicsPipeline,
            commandBuffers[currentFrame],
            vertexBuffer,
            indexBuffer,
            renderPass,
            swapChainExtent,
            swapChainFramebuffers
    };
    Renderer::recordCommandBuffer(recordCommandBufferInfo);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        Renderer::RecreateSwapChainData recreateSwapChainData{
                Renderer::CreateSwapChainInfo{
                        physicalDevice,
                        device,
                        surface,
                        window,
                        swapChain,
                        swapChainImages,
                        swapChainImageFormat,
                        swapChainExtent,
                },
                graphicsPipeline,
                pipelineLayout,
                renderPass,
                swapChainImageViews,
                swapChainFramebuffers,
        };
        Renderer::recreateSwapChain(recreateSwapChainData);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanApplication::cleanup() {
    Renderer::cleanupSwapChain(device, graphicsPipeline, pipelineLayout, renderPass, swapChain,
                               swapChainImageViews, swapChainFramebuffers);

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}



