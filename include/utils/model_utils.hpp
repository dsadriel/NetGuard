#ifndef _MODEL_UTILS_HPP
#define _MODEL_UTILS_HPP

#include <glad/glad.h>
#include "core/scene.hpp"
#include <map>
#include <string>


extern map<string, SceneObject> g_VirtualScene;

GLuint BuildTriangles();

#endif