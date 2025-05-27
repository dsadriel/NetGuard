#ifndef _MOUSE_KEYBOARD_H
#define _MOUSE_KEYBOARD_H

#include <glfw/glfw3.h>
#include <game/Camera.h>


extern float g_ScreenRatio;
extern bool g_UsePerspectiveProjection;
extern bool g_LeftMouseButtonPressed;
extern float g_MovementSpeed;
extern float g_MouseSensitivity;
extern double g_LastCursorPosX;
extern double g_LastCursorPosY;

extern Camera g_Camera;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
// void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);

#endif