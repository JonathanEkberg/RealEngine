#pragma once

#include "renderer/context.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <optional>

using std::optional;
using std::vector;

class VulkanApplication {
public:
    void run();

private:
    static const size_t WIDTH = 1920;
    static const size_t HEIGHT = 1080;

    Renderer::Context ctx;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void drawFrame();

    void cleanup();
};

