#pragma once

#include "renderer/instance.hpp"
#include "renderer/surface.hpp"
#include "renderer/device.hpp"
#include "renderer/render_pass.hpp"
#include "renderer/pipeline.hpp"
#include "renderer/frame_buffer.hpp"
#include "renderer/command_pool.hpp"
#include "renderer/command_buffer.hpp"
#include "renderer/synchronization.hpp"

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

