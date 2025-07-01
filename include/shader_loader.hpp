#ifndef SHADERLOADER_HPP_
#define SHADERLOADER_HPP_

#include <external/glad/glad.h>   
#include <external/GLFW/glfw3.h> 
#include <external/glm/gtc/type_ptr.hpp>

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
