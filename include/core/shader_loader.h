#ifndef SHADERLOADER_H_
#define SHADERLOADER_H_

#include <glad/glad.h>   
#include <GLFW/glfw3.h> 
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>


void LoadShadersFromFiles(GLuint* gpu_program_id);
GLuint LoadShader_Vertex(const char *filename);
GLuint LoadShader_Fragment(const char *filename);
void LoadShader(const char *filename, GLuint shader_id);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); 

#endif // SHADERLOADER_H_
