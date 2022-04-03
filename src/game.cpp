#include "game.hpp"

void VulkanApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanApplication::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    std::cout << "Vulkan supported?: " << glfwVulkanSupported() << std::endl;

    ctx.window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void VulkanApplication::initVulkan() {
    Renderer::createInstance(&ctx);
    Renderer::createSurface(&ctx);
    Renderer::pickPhysicalDevice(&ctx);
    Renderer::createLogicalDevice(&ctx);
    Renderer::createSwapChain(&ctx);
    Renderer::createImageViews(&ctx);
    Renderer::createRenderPass(&ctx);
    Renderer::createGraphicsPipeline(&ctx);
    Renderer::createFramebuffers(&ctx);
    Renderer::createCommandPool(&ctx);
    Renderer::createCommandBuffer(&ctx);
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
    vkWaitForFences(ctx.device, 1, &ctx.inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(ctx.device, 1, &ctx.inFlightFence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(ctx.device, ctx.swapChain, UINT64_MAX, ctx.imageAvailableSemaphore, VK_NULL_HANDLE,
                          &imageIndex);

    vkResetCommandBuffer(ctx.commandBuffer, 0);
    Renderer::recordCommandBuffer(&ctx, ctx.commandBuffer, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {ctx.imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &ctx.commandBuffer;

    VkSemaphore signalSemaphores[] = {ctx.renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(ctx.graphicsQueue, 1, &submitInfo, ctx.inFlightFence) != VK_SUCCESS) {
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

    vkQueuePresentKHR(ctx.graphicsQueue, &presentInfo);
}

void VulkanApplication::cleanup() {
    vkDestroySemaphore(ctx.device, ctx.imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(ctx.device, ctx.renderFinishedSemaphore, nullptr);
    vkDestroyFence(ctx.device, ctx.inFlightFence, nullptr);

    vkDestroyCommandPool(ctx.device, ctx.commandPool, nullptr);

    for (auto framebuffer: ctx.swapChainFramebuffers) {
        vkDestroyFramebuffer(ctx.device, framebuffer, nullptr);
    }

    vkDestroyRenderPass(ctx.device, ctx.renderPass, nullptr);
    vkDestroyPipeline(ctx.device, ctx.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(ctx.device, ctx.pipelineLayout, nullptr);

    for (auto imageView: ctx.swapChainImageViews) {
        vkDestroyImageView(ctx.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(ctx.device, ctx.swapChain, nullptr);
    vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
    vkDestroyDevice(ctx.device, nullptr);
    vkDestroyInstance(ctx.instance, nullptr);

    glfwDestroyWindow(ctx.window);

    glfwTerminate();
}
