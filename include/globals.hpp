#if !defined(GLOBAL_HPP)
#define GLOBAL_HPP

#include <map>
#include <string>

using std::map;
using std::string;

// Forward declaration to avoid circular dependency
class NetGuard;
class SceneObject;

extern NetGuard g_NetGuard;
extern map<string, SceneObject> g_VirtualScene;
extern float g_ScreenWidth, g_ScreenHeight, g_ScreenRatio, g_MouseSensitivity;
extern float g_WindowWidth, g_WindowHeight;
extern bool g_LeftMouseButtonPressed;
extern double g_LastCursorPosX, g_LastCursorPosY;

#endif // GLOBAL_HPP
