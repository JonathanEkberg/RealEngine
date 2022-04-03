#include "frame_buffer.hpp"

void Renderer::createFramebuffers(Context *context) {
    context->swapChainFramebuffers.resize(context->swapChainImageViews.size());

    for (size_t i = 0; i < context->swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
                context->swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = context->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = context->swapChainExtent.width;
        framebufferInfo.height = context->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(context->device, &framebufferInfo, nullptr, &context->swapChainFramebuffers[i]) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}
