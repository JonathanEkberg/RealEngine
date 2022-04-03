#pragma once

#include "renderer/instance.h"
#include "renderer/surface.h"
#include "renderer/device.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/frame_buffer.h"
#include "renderer/command_pool.h"
#include "renderer/command_buffer.h"
#include "renderer/synchronization.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <optional>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>

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

