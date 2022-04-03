#include "synchronization.hpp"

void Renderer::createSyncObjects(Renderer::Context *context) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    auto createImageSemaphoreSuccess =
            vkCreateSemaphore(context->device, &semaphoreInfo, nullptr, &context->imageAvailableSemaphore) ==
            VK_SUCCESS;
    if (!createImageSemaphoreSuccess) {
        throw std::runtime_error("Failed to create image available semaphore!");
    }

    auto createRenderSemaphore =
            vkCreateSemaphore(context->device, &semaphoreInfo, nullptr, &context->renderFinishedSemaphore) ==
            VK_SUCCESS;
    if (!createRenderSemaphore) {
        throw std::runtime_error("Failed to create render finished semaphore!");
    }

    auto createFenceSuccess =
            vkCreateFence(context->device, &fenceInfo, nullptr, &context->inFlightFence) == VK_SUCCESS;
    if (!createFenceSuccess) {
        throw std::runtime_error("Failed to in flight fence!");
    }
}
