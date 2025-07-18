// clang-format off
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

#include <external/glad/glad.h>   
#define GLFW_INCLUDE_NONE
#include <external/GLFW/glfw3.h> 
#include <external/glm/gtc/type_ptr.hpp>

#include "shader_loader.hpp"
#include "scene.hpp"
#include "globals.hpp"
#include "utils/text_rendering.hpp"
#include "utils/model_utils.hpp"
#include "input/mouse_keyboard_callbacks.hpp"
#include "input/window_callbacks.hpp"
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
// Framebuffer size (actual pixels)
float g_ScreenWidth = 1024.0f;
float g_ScreenHeight = 768.0f;
float g_ScreenRatio = 1024.0f / 768.0f;
// Window size (logical coordinates)
float g_WindowWidth = 1024.0f;
float g_WindowHeight = 768.0f;
bool g_LeftMouseButtonPressed = false;
float g_MouseSensitivity = 0.005f;
double g_LastCursorPosX, g_LastCursorPosY;

double g_LastFrameTime = 0.0;
double g_CurrentFrameTime = 0.0;
float g_DeltaTime = 0.0f;

bool g_DebugMode = false;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;
GLint g_shading_mode_uniform;
GLint g_object_style_uniform;
GLint g_object_color_uniform;
GLint g_texture0_uniform;
GLint g_texture1_uniform;
GLint g_texture2_uniform;

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

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);
	glfwSetWindowSize(window, 1024, 768);
	glfwMakeContextCurrent(window);

	initializeWindowDimensions(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *glversion = glGetString(GL_VERSION);
	const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

	LoadShadersFromFiles();

	TextRendering_Init();

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
	// ==================================================

	ObjModel mapModel("./assets/models/map.obj");
	ComputeNormals(&mapModel);
	BuildTrianglesAndAddToVirtualScene(&mapModel);

	ObjModel boardModel("./assets/models/board.obj");
	ComputeNormals(&boardModel);
	BuildTrianglesAndAddToVirtualScene(&boardModel);

	ObjModel neoCatModel("./assets/models/neocat/neocat.obj");
	ComputeNormals(&neoCatModel);
	BuildTrianglesAndAddToVirtualScene(&neoCatModel);

	ObjModel plane1x1Model("./assets/models/plane1x1.obj");
	ComputeNormals(&plane1x1Model);
	BuildTrianglesAndAddToVirtualScene(&plane1x1Model);
	g_VirtualScene["plane1x1"].shading_mode = SHADING_GOURAUD;

	ObjModel skyBoxModel("./assets/models/skybox.obj");
	ComputeNormals(&skyBoxModel);
	BuildTrianglesAndAddToVirtualScene(&skyBoxModel);

	ObjModel antivirusModel("./assets/models/antivirus/antivirus.obj");
	ComputeNormals(&antivirusModel);
	BuildTrianglesAndAddToVirtualScene(&antivirusModel);

	// Link all scene objects to NetGuard
	g_NetGuard.linkSceneObjects(&g_VirtualScene["map"], &g_VirtualScene["board"], &g_VirtualScene["neocat"],
	                            &g_VirtualScene["plane1x1"], &g_VirtualScene["antivirus"]);

	// ==================================================
	// MARK: Carrega texturas
	// ==================================================

	// Carrega a textura do neocat
	g_VirtualScene["neocat"].applyTexture("./assets/textures/neocat.png");

	g_VirtualScene["map"].applyTexture("./assets/textures/grid.png");
	g_VirtualScene["map"].object_style = FLAT_TEXTURED;
	g_VirtualScene["map"].shading_mode = SHADING_PHONG;

	g_VirtualScene["board"].applyTexture("./assets/textures/grid.png");
	g_VirtualScene["board"].object_style = FLAT_TEXTURED;
	g_VirtualScene["board"].scale = glm::vec3(1.0f, 1.0f, 1.0f);
	g_VirtualScene["board"].position = glm::vec4(0.0f, 0.01f, 0.0f, 1.0f);
	g_VirtualScene["board"].shading_mode = SHADING_PHONG;

	g_VirtualScene["antivirus"].applyTexture("./assets/textures/antivirus.png");
	g_VirtualScene["antivirus"].scale = glm::vec3(0.6f, 0.6f, 0.6f);
	g_VirtualScene["antivirus"].shading_mode = SHADING_PHONG;

	g_VirtualScene["skybox"].applyTexture("./assets/textures/skybox.png");
	g_VirtualScene["skybox"].object_style = PLAIN_TEXTURED;

	// Configura os uniforms das texturas
	glUseProgram(g_GpuProgramID);
	glUniform1i(g_texture0_uniform, 0);
	glUniform1i(g_texture1_uniform, 1);
	glUniform1i(g_texture2_uniform, 2);

	// Configura a instância do NetGuard
	g_NetGuard.link(window, g_model_uniform, g_object_style_uniform, g_object_color_uniform, g_shading_mode_uniform);

	// ==================================================
	// MARK: Loop Principal
	// O loop principal do programa começa aqui.
	// ==================================================
	g_LastFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		g_CurrentFrameTime = glfwGetTime();
		g_DeltaTime = (float)(g_CurrentFrameTime - g_LastFrameTime);
		g_LastFrameTime = g_CurrentFrameTime;

		glClearColor(39 / 255.0f, 37 / 255.0f, 38 / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(g_GpuProgramID);

		glm::mat4 view = g_NetGuard.camera.getViewMatrix();
		glm::mat4 projection = g_NetGuard.camera.getProjectionMatrix(g_ScreenRatio);

		glUniformMatrix4fv(g_view_uniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(g_projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));

		// ==================================================
		// MARK: Atualiza e desenha o jogo
		// ==================================================

		// Desenha o skybox primeiro (sempre atrás de tudo)
		glDepthMask(GL_FALSE); // Não escreve no depth buffer
		g_VirtualScene["skybox"].position =
		    glm::vec4(g_NetGuard.camera.position.x, g_NetGuard.camera.position.y, g_NetGuard.camera.position.z, 1.0f);
		g_VirtualScene["skybox"].drawObject(g_model_uniform, g_object_style_uniform, g_object_color_uniform,
		                                    g_shading_mode_uniform);
		glDepthMask(GL_TRUE); // Volta a escrever no depth buffer

		g_NetGuard.update(g_DeltaTime);

		g_NetGuard.draw();

		glBindVertexArray(0);

		if (g_DebugMode) {
			TextRendering_ShowFramesPerSecond(window);
			std::string currentStage = g_NetGuard.getCurrentStageString();
			TextRendering_PrintStringC(window, currentStage, -0.999f, 0.95f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}