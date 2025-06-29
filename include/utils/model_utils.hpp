#ifndef _MODEL_UTILS_HPP
#define _MODEL_UTILS_HPP

#include <external/glad/glad.h>
#include "core/scene.hpp"
#include <map>
#include <string>


extern map<string, SceneObject> g_VirtualScene;

void DrawVirtualObject(const char *object_name);

#endif