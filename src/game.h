#pragma once

#include "renderer/context.h"

#include <optional>

using std::optional;
using std::vector;

class VulkanApplication {
public:
    void run();

private:
    static constexpr size_t WIDTH = 1920;
    static constexpr size_t HEIGHT = 1080;

    Renderer::Context ctx;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void drawFrame();

    void cleanup();
};

