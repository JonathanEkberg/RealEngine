#pragma once

#include <stdexcept>
#include "context.hpp"

namespace Renderer {
    void createCommandBuffer(Context *ctx);

    void recordCommandBuffer(Context *ctx, VkCommandBuffer commandBuffer, uint32_t imageIndex);
}