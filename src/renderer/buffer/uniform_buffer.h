#include <glm/glm.hpp>

namespace Renderer {
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    void createDescriptorSetLayout(VkDescriptorSetLayout &descriptorSetLayout);
}