// clang-format off
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

#include <glad/glad.h>   
#include <GLFW/glfw3.h> 
#include <glm/gtc/type_ptr.hpp>

#include "core/shader_loader.hpp"
#include "core/scene.hpp"
#include "utils/text_rendering.hpp"
#include "utils/model_utils.hpp"
#include "input/mouse_keyboard_callbacks.hpp"
#include "matrices.hpp"
#include "game/camera.hpp"
#include "utils/obj_loader_utils.hpp"

using namespace std;

// clang-format on
// ==================================================
// MARK: Variáveis globais
// Todas as as variáveis globais devem ser declaradas aqui com o prefixo "g_"
// ==================================================

Camera g_Camera = Camera(glm::vec4(2.0f, 2.0f, 2.0f, 1.0f), // Posicao incial da camera
                         -2.4f,                             // angulo de yaw (em radianos)
                         -0.5f                              // angulo de pitch (em radianos)
);
map<string, SceneObject> g_VirtualScene;
float g_ScreenRatio = 1024.0f / 768.0f;
bool g_UsePerspectiveProjection = true;
bool g_LeftMouseButtonPressed = false;
float g_MovementSpeed = 0.05f;
float g_MouseSensitivity = 0.005f;
double g_LastCursorPosX, g_LastCursorPosY;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;

// ==================================================
// MARK: Programa Principal
// ==================================================

int main() {
	int success = glfwInit();
	if (!success) {
		fprintf(stderr, "ERROR: glfwInit() failed.\n");
		exit(EXIT_FAILURE);
	}

	// glfwSetErrorCallback(ErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window;
	window = glfwCreateWindow(1024, 768, "NetGuard: In a world of chaos, be the firewall", NULL, NULL);
	if (!window) {
		glfwTerminate();
		fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	// glfwSetScrollCallback(window, ScrollCallback); 

	// glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetWindowSize(window, 1024, 768);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *glversion = glGetString(GL_VERSION);
	const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

	LoadShadersFromFiles();

	TextRendering_Init();

	GLint model_uniform = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
	GLint view_uniform =
	    glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
	GLint projection_uniform =
	    glGetUniformLocation(g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
	GLint render_as_black_uniform =
	    glGetUniformLocation(g_GpuProgramID, "render_as_black"); // Variável booleana em shader_vertex.glsl

	// Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
	glEnable(GL_DEPTH_TEST);

	// Variáveis auxiliares utilizadas para chamada à função
	// TextRendering_ShowModelViewProjection(), armazenando matrizes 4x4.
	glm::mat4 the_projection;
	glm::mat4 the_model;
	glm::mat4 the_view;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	TextRendering_Init();

	ObjModel bunnymodel("../../assets/models/map.obj");
	ComputeNormals(&bunnymodel);
	BuildTrianglesAndAddToVirtualScene(&bunnymodel, &g_VirtualScene);

	// ==================================================
	// MARK: Loop Principal
	// O loop principal do programa começa aqui.
	// ==================================================

	while (!glfwWindowShouldClose(window)) {

		glClearColor(39 / 255.0f, 37 / 255.0f, 38 / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(g_GpuProgramID);

		glm::mat4 view = g_Camera.getViewMatrix();

		// Vamos definir a matriz de projeção.
		glm::mat4 projection;
		// Projeção Perspectiva.
		float nearplane = -0.1f; // Posição do "near plane"
		float farplane = -10.0f; // Posição do "far plane"
		float field_of_view = 3.141592 / 3.0f;
		projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));

		// ==================================================
		// MARK: Desenho dos objetos
		// ==================================================

		glm::mat4 model = Matrix_Identity()*Matrix_Scale(0.1f, 0.1f, 0.1f); // Transformação identidade de modelagem
		glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(g_object_id_uniform, 0);
		DrawVirtualObject("map");

		// Agora queremos desenhar os eixos XYZ de coordenadas GLOBAIS.
		{
			glm::mat4 model = Matrix_Identity();
			glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
			glLineWidth(10.0f);
			glUniform1i(render_as_black_uniform, false);
			glDrawElements(g_VirtualScene["axes"].rendering_mode, g_VirtualScene["axes"].num_indices, GL_UNSIGNED_INT,
			               (void *)g_VirtualScene["axes"].first_index);
		}

		glBindVertexArray(0);
		TextRendering_ShowFramesPerSecond(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}