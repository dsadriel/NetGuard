#ifndef SHADERLOADER_H_
#define SHADERLOADER_H_

#include <glad/glad.h>   
#include <GLFW/glfw3.h> 
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

// Variáveis que definem um programa de GPU (shaders) e ESTÃO NA MAIN. Veja função LoadShadersFromFiles().
extern GLuint g_GpuProgramID;
extern GLint g_model_uniform;
extern GLint g_view_uniform;
extern GLint g_projection_uniform;
extern GLint g_object_id_uniform;

void LoadShadersFromFiles();
GLuint LoadShader_Vertex(const char *filename);
GLuint LoadShader_Fragment(const char *filename);
void LoadShader(const char *filename, GLuint shader_id);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); 

#endif // SHADERLOADER_H_
