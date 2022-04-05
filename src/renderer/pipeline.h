#pragma once

#include "context.h"

namespace Renderer {
    void createGraphicsPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass,
                                VkExtent2D swapChainExtent, VkPipeline &graphicsPipeline);
}
