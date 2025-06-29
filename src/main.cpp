// clang-format off
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

#include <external/glad/glad.h>   
#include <GLFW/glfw3.h> 
#include <external/glm/gtc/type_ptr.hpp>

#include "shader_loader.hpp"
#include "scene.hpp"
#include "utils/text_rendering.hpp"
#include "utils/model_utils.hpp"
#include "input/mouse_keyboard_callbacks.hpp"
#include "matrices.hpp"
#include "game/net_guard.hpp"
#include "utils/obj_loader_utils.hpp"

using namespace std;

// clang-format on
// ==================================================
// MARK: Variáveis globais
// Todas as as variáveis globais devem ser declaradas aqui com o prefixo "g_"
// ==================================================

NetGuard g_NetGuard = NetGuard();
map<string, SceneObject> g_VirtualScene;
float g_ScreenRatio = 1024.0f / 768.0f;
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
	#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

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

	// GLint model_uniform = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
	GLint view_uniform =
	    glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
	GLint projection_uniform =
	    glGetUniformLocation(g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
	// GLint render_as_black_uniform =
	// glGetUniformLocation(g_GpuProgramID, "render_as_black"); // Variável booleana em shader_vertex.glsl

	// Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
	glEnable(GL_DEPTH_TEST);

	// Variáveis auxiliares utilizadas para chamada à função
	// TextRendering_ShowModelViewProjection(), armazenando matrizes 4x4.
	// glm::mat4 the_projection;
	// glm::mat4 the_model;
	// glm::mat4 the_view;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	TextRendering_Init();

	// ==================================================
	// MARK: Carrega os objetos
	//
	// ==================================================
	
	ObjModel mapModel("../../assets/models/map.obj");
	ComputeNormals(&mapModel);
	BuildTrianglesAndAddToVirtualScene(&mapModel);

	ObjModel neoCatModel("../../assets/models/neocat/neocat.obj");
	ComputeNormals(&neoCatModel);
	BuildTrianglesAndAddToVirtualScene(&neoCatModel);

	ObjModel thePlaneModel("../../assets/models/plane.obj");
	ComputeNormals(&thePlaneModel);
	BuildTrianglesAndAddToVirtualScene(&thePlaneModel);

	// ==================================================
	// MARK: Loop Principal
	// O loop principal do programa começa aqui.
	// ==================================================

	while (!glfwWindowShouldClose(window)) {

		glClearColor(39 / 255.0f, 37 / 255.0f, 38 / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(g_GpuProgramID);

		glm::mat4 view = g_NetGuard.camera.getViewMatrix();
		glm::mat4 projection = g_NetGuard.camera.getProjectionMatrix(g_ScreenRatio);

		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));

		// ==================================================
		// MARK: Atualiza e desenha o jogo
		// ==================================================

		g_NetGuard.update(0.016f); // Atualiza o estado do jogo a cada frame (60 FPS)
		g_NetGuard.draw(); // Desenha o estado atual do jogo


		g_VirtualScene["map"].scale = glm::vec3(1.0f, 1.0f, 1.0f);
		g_VirtualScene["map"].drawObject(g_model_uniform, g_object_id_uniform, 0);

		g_VirtualScene["the_plane"].position = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
		g_VirtualScene["the_plane"].drawObject(g_model_uniform, g_object_id_uniform, 1);

		g_VirtualScene["neocat"].position = glm::vec4(0.0f, 1.5f, 0.0f, 1.0f);
		g_VirtualScene["neocat"].drawObject(g_model_uniform, g_object_id_uniform, 2);


		glBindVertexArray(0);

		// Shows current FPS and stage
		TextRendering_ShowFramesPerSecond(window);
		std::string currentStage = g_NetGuard.getCurrentStageString();
		TextRendering_PrintStringC(window, currentStage, -0.999f, 0.95f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}


