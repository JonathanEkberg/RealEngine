#include "synchronization.h"
#include "constants.h"

void Renderer::createSyncObjects(Context *ctx) {
    ctx->imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    ctx->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    ctx->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        auto createImageSemaphoreSuccess =
                vkCreateSemaphore(ctx->device, &semaphoreInfo, nullptr, &ctx->imageAvailableSemaphores[i]) ==
                VK_SUCCESS;
        if (!createImageSemaphoreSuccess) {
            throw std::runtime_error("Failed to create image available semaphore!");
        }

        auto createRenderSemaphore =
                vkCreateSemaphore(ctx->device, &semaphoreInfo, nullptr, &ctx->renderFinishedSemaphores[i]) ==
                VK_SUCCESS;
        if (!createRenderSemaphore) {
            throw std::runtime_error("Failed to create render finished semaphore!");
        }

        auto createFenceSuccess =
                vkCreateFence(ctx->device, &fenceInfo, nullptr, &ctx->inFlightFences[i]) == VK_SUCCESS;
        if (!createFenceSuccess) {
            throw std::runtime_error("Failed to in flight fence!");
        }
    }
}
