#include "game.h"
#include "renderer/instance.h"
#include "renderer/surface.h"
#include "renderer/device.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/frame_buffer.h"
#include "renderer/command_pool.h"
#include "renderer/vertex.h"
#include "renderer/index_buffer.h"
#include "renderer/command_buffer.h"
#include "renderer/synchronization.h"
#include "renderer/constants.h"

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

    ctx.window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(ctx.window, this);
    glfwSetWindowSizeCallback(ctx.window, [](GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<VulkanApplication *>(glfwGetWindowUserPointer(window));
        app->ctx.framebufferResized = true;
    });
}

void VulkanApplication::initVulkan() {
    Renderer::createInstance(&ctx);
    Renderer::createSurface(&ctx);
    Renderer::pickPhysicalDevice(ctx.instance, ctx.surface, &ctx.physicalDevice);
    Renderer::createLogicalDevice(ctx.physicalDevice, ctx.surface, &ctx.device, &ctx.graphicsQueue, &ctx.presentQueue);
    Renderer::createSwapChain(&ctx);
    Renderer::createImageViews(&ctx);
    Renderer::createRenderPass(&ctx);
    Renderer::createGraphicsPipeline(&ctx);
    Renderer::createFramebuffers(&ctx);
    Renderer::createCommandPool(&ctx);
    Renderer::createVertexBuffer(&ctx);
    Renderer::createIndexBuffer(&ctx);
    Renderer::createCommandBuffers(ctx.device, ctx.commandPool, &ctx.commandBuffers);
    Renderer::createSyncObjects(&ctx);
}

void VulkanApplication::mainLoop() {
    while (!glfwWindowShouldClose(ctx.window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(ctx.device);
}

void VulkanApplication::drawFrame() {
    vkWaitForFences(ctx.device, 1, &ctx.inFlightFences[ctx.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(ctx.device, ctx.swapChain, UINT64_MAX,
                                            ctx.imageAvailableSemaphores[ctx.currentFrame],
                                            VK_NULL_HANDLE,
                                            &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || ctx.framebufferResized) {
        ctx.framebufferResized = false;
        Renderer::recreateSwapChain(&ctx);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to aquire swap chain image!");
    }

    vkResetFences(ctx.device, 1, &ctx.inFlightFences[ctx.currentFrame]);
    auto test = ctx.commandBuffers[ctx.currentFrame];

    vkResetCommandBuffer(ctx.commandBuffers[ctx.currentFrame], 0);
//    Renderer::recordCommandBuffer(&ctx, ctx.commandBuffers[ctx.currentFrame], imageIndex);
    Renderer::recordCommandBuffer(imageIndex, ctx.graphicsPipeline, ctx.commandBuffers[ctx.currentFrame],
                                  ctx.vertexBuffer, ctx.indexBuffer, ctx.renderPass, ctx.swapChainExtent,
                                  ctx.swapChainFramebuffers);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {ctx.imageAvailableSemaphores[ctx.currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &ctx.commandBuffers[ctx.currentFrame];

    VkSemaphore signalSemaphores[] = {ctx.renderFinishedSemaphores[ctx.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(ctx.graphicsQueue, 1, &submitInfo, ctx.inFlightFences[ctx.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {ctx.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(ctx.graphicsQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        Renderer::recreateSwapChain(&ctx);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    ctx.currentFrame = (ctx.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanApplication::cleanup() {
    Renderer::cleanupSwapChain(&ctx);

    vkDestroyBuffer(ctx.device, ctx.indexBuffer, nullptr);
    vkFreeMemory(ctx.device, ctx.indexBufferMemory, nullptr);

    vkDestroyBuffer(ctx.device, ctx.vertexBuffer, nullptr);
    vkFreeMemory(ctx.device, ctx.vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(ctx.device, ctx.imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(ctx.device, ctx.renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(ctx.device, ctx.inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(ctx.device, ctx.commandPool, nullptr);

    vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
    vkDestroyDevice(ctx.device, nullptr);
    vkDestroyInstance(ctx.instance, nullptr);

    glfwDestroyWindow(ctx.window);

    glfwTerminate();
}
