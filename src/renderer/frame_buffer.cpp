#include "frame_buffer.hpp"

void Renderer::createFramebuffers(Context *ctx) {
    ctx->swapChainFramebuffers.resize(ctx->swapChainImageViews.size());

    for (size_t i = 0; i < ctx->swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
                ctx->swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = ctx->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = ctx->swapChainExtent.width;
        framebufferInfo.height = ctx->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(ctx->device, &framebufferInfo, nullptr, &ctx->swapChainFramebuffers[i]) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}
