// text_rendering.hpp
// Header for OpenGL-based text rendering (no external font libraries)

#ifndef TEXT_RENDERING_HPP
#define TEXT_RENDERING_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>


GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); 

void TextRendering_LoadShader(const GLchar* const shader_string, GLuint shader_id);
void TextRendering_Init();
void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale);
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintMatrix(GLFWwindow* window, glm::mat4 M, float x, float y, float scale);
void TextRendering_PrintVector(GLFWwindow* window, glm::vec4 v, float x, float y, float scale);
void TextRendering_PrintMatrixVectorProduct(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale);
void TextRendering_PrintMatrixVectorProductMoreDigits(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale);
void TextRendering_PrintMatrixVectorProductDivW(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale);
void TextRendering_ShowFramesPerSecond(GLFWwindow* window);

#endif // TEXT_RENDERING_HPP
