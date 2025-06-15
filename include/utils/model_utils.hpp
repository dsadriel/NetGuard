#ifndef _MODEL_UTILS_HPP
#define _MODEL_UTILS_HPP

#include <glad/glad.h>
#include "core/scene.hpp"
#include "stb_image.h"
#include <map>
#include <string>
#include <iostream>


extern map<string, SceneObject> g_VirtualScene;
extern GLuint mapTexture;
void DrawVirtualObject(const char *object_name);
GLuint LoadTextureFromFile(const std::string& filename);


#endif