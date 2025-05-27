#ifndef _MODEL_UTILS_H
#define _MODEL_UTILS_H

#include <glad/glad.h>
#include <core/scene.h>
#include <map>
#include <string>


extern map<string, SceneObject> g_VirtualScene;

GLuint BuildTriangles();

#endif