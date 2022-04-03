#pragma once

#include <stdexcept>
#include "context.h"

namespace Renderer {
    void createCommandBuffers(Context *ctx);

    void recordCommandBuffer(Context *ctx, VkCommandBuffer commandBuffer, uint32_t imageIndex);
}