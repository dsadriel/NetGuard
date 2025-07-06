#include "input/window_callbacks.hpp"
#include "globals.hpp"
#include <external/glad/glad.h>
#include <cstdio>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    g_ScreenWidth = (float)width;
    g_ScreenHeight = (float)height;
    g_ScreenRatio = g_ScreenWidth / g_ScreenHeight;
    glViewport(0, 0, width, height);
    printf("Framebuffer resized to: %dx%d, ratio: %.3f\n", width, height, g_ScreenRatio);
}

void WindowSizeCallback(GLFWwindow* window, int width, int height) {
    g_WindowWidth = (float)width;
    g_WindowHeight = (float)height;
    printf("Window resized to: %dx%d\n", width, height);
}

void initializeWindowDimensions(GLFWwindow* window) {
    if (!window) return;

    int window_width, window_height;
    int framebuffer_width, framebuffer_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    
    g_WindowWidth = (float)window_width;
    g_WindowHeight = (float)window_height;
    g_ScreenWidth = (float)framebuffer_width;
    g_ScreenHeight = (float)framebuffer_height;
    g_ScreenRatio = (g_ScreenHeight > 0.0f) ? (g_ScreenWidth / g_ScreenHeight) : 1.0f;
    
    printf("Initial window size: %dx%d, framebuffer size: %dx%d\n", 
           window_width, window_height, framebuffer_width, framebuffer_height);
    
    if (g_WindowWidth > 0.0f && g_WindowHeight > 0.0f) {
        float scale_factor = (g_ScreenWidth / g_WindowWidth + g_ScreenHeight / g_WindowHeight) / 2.0f;
        if (scale_factor > 1.0f) {
            printf("High-DPI display detected with scale factor: %.1fx\n", scale_factor);
        }
    }
}
