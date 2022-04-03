#include "context.h"
#include "device.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Renderer {
    void createBuffer(Context *ctx, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
}