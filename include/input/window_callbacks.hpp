#ifndef INPUT_WINDOW_CALLBACKS_HPP
#define INPUT_WINDOW_CALLBACKS_HPP

#define GLFW_INCLUDE_NONE
#include <external/GLFW/glfw3.h>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void WindowSizeCallback(GLFWwindow* window, int width, int height);
void initializeWindowDimensions(GLFWwindow* window);

#endif // INPUT_WINDOW_CALLBACKS_HPP
