#pragma once

#include "context.hpp"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

namespace Renderer {
    void createRenderPass(Context *context);
}