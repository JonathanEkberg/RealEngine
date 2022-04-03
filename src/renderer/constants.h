#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include "vertex.h"

using std::vector;

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 3;

const vector<Renderer::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}}};
const vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
};

const vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif
