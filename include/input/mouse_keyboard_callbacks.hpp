#ifndef _MOUSE_KEYBOARD_HPP
#define _MOUSE_KEYBOARD_HPP

#define GLFW_INCLUDE_NONE
#include <external/GLFW/glfw3.h>
#include "game/net_guard.hpp"
#include "globals.hpp"


extern bool g_UsePerspectiveProjection;
extern bool g_LeftMouseButtonPressed;
extern float g_MovementSpeed;
extern float g_MouseSensitivity;
extern double g_LastCursorPosX;
extern double g_LastCursorPosY;

extern NetGuard g_NetGuard;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
// void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);

#endif