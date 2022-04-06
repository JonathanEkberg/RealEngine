#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace Renderer {
    void createSyncObjects(VkDevice device, std::vector<VkSemaphore> &imageAvailableSemaphores,
                           std::vector<VkSemaphore> &renderFinishedSemaphores,
                           std::vector<VkFence> &inFlightFences);
}
