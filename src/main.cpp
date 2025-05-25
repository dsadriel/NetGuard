// clang-format off
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

#include <glad/glad.h>   
#include <GLFW/glfw3.h> 
#include <glm/gtc/type_ptr.hpp>

#include "core/shader_loader.h"
#include "utils/text_rendering.h"
#include "matrices.h"
#include "game/Camera.h"

// clang-format on

// ==================================================
// MARK: Protótipos de funções
// ==================================================

void TextRendering_ShowFramesPerSecond(GLFWwindow *window);
GLuint BuildTriangles();
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

// ==================================================
// MARK: Definições de estruturas
// ==================================================

struct SceneObject {
	const char *name;      // Nome do objeto
	void *first_index;     // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTriangles()
	int num_indices;       // Número de índices do objeto dentro do vetor indices[] definido em BuildTriangles()
	GLenum rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
};

// ==================================================
// MARK: Variáveis globais
// Todas as as variáveis globais devem ser declaradas aqui com o prefixo "g_"
// ==================================================

Camera g_Camera = Camera(glm::vec4(2.0f, 2.0f, 2.0f, 1.0f), // Posicao incial da camera
                         -2.4f,                             // angulo de yaw (em radianos)
                         -0.5f                              // angulo de pitch (em radianos)
);
std::map<const char *, SceneObject> g_VirtualScene;
float g_ScreenRatio = 1024.0f / 768.0f;
bool g_UsePerspectiveProjection = true;
GLuint g_GpuProgramID = 0;
bool g_LeftMouseButtonPressed = false;
float g_MovementSpeed = 0.05f;
float g_MouseSensitivity = 0.005f;
double g_LastCursorPosX, g_LastCursorPosY;

// ==================================================
// MARK: Programa Principal
// ==================================================

int main() {
	int success = glfwInit();
	if (!success) {
		fprintf(stderr, "ERROR: glfwInit() failed.\n");
		std::exit(EXIT_FAILURE);
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
		std::exit(EXIT_FAILURE);
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

	LoadShadersFromFiles(&g_GpuProgramID);

	GLuint vertex_array_object_id = BuildTriangles();

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

	// ==================================================
	// MARK: Loop Principal
	// O loop principal do programa começa aqui.
	// ==================================================

	while (!glfwWindowShouldClose(window)) {

		glClearColor(39 / 255.0f, 37 / 255.0f, 38 / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(g_GpuProgramID);

		glBindVertexArray(vertex_array_object_id);

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

		// Vamos desenhar 3 instâncias (cópias) do cubo
		for (int i = 1; i <= 3; ++i) {
			// Cada cópia do cubo possui uma matriz de modelagem independente,
			// já que cada cópia estará em uma posição (rotação, escala, ...)
			// diferente em relação ao espaço global (World Coordinates). Veja
			// slides 2-14 e 184-190 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
			glm::mat4 model;

			if (i == 1) {
				// A primeira cópia do cubo não sofrerá nenhuma transformação
				// de modelagem. Portanto, sua matriz "model" é a identidade, e
				// suas coordenadas no espaço global (World Coordinates) serão
				// *exatamente iguais* a suas coordenadas no espaço do modelo
				// (Model Coordinates).
				model = Matrix_Identity();
			} else if (i == 2) {
				// A segunda cópia do cubo sofrerá um escalamento não-uniforme,
				// seguido de uma rotação no eixo (1,1,1), e uma translação em Z (nessa ordem!).
				model = Matrix_Translate(0.0f, 0.0f, -2.0f)                                  // TERCEIRO translação
				        * Matrix_Rotate(3.141592f / 8.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)) // SEGUNDO rotação
				        * Matrix_Scale(2.0f, 0.5f, 0.5f);                                    // PRIMEIRO escala
			} else if (i == 3) {
				// A terceira cópia do cubo sofrerá rotações em X,Y e Z (nessa
				// ordem) seguindo o sistema de ângulos de Euler, e após uma
				// translação em X. Veja slides 106-107 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
				model = Matrix_Translate(-2.0f, 0.0f, 0.0f); // QUARTO translação

				// Armazenamos as matrizes model, view, e projection do terceiro cubo
				// para mostrar elas na tela através da função TextRendering_ShowModelViewProjection().
				the_model = model;
				the_projection = projection;
				the_view = view;
			}

			// Enviamos a matriz "model" para a placa de vídeo (GPU). Veja o
			// arquivo "shader_vertex.glsl", onde esta é efetivamente
			// aplicada em todos os pontos.
			glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));

			// Informamos para a placa de vídeo (GPU) que a variável booleana
			// "render_as_black" deve ser colocada como "false". Veja o arquivo
			// "shader_vertex.glsl".
			glUniform1i(render_as_black_uniform, false);

			// Pedimos para a GPU rasterizar os vértices do cubo apontados pelo
			// VAO como triângulos, formando as faces do cubo. Esta
			// renderização irá executar o Vertex Shader definido no arquivo
			// "shader_vertex.glsl", e o mesmo irá utilizar as matrizes
			// "model", "view" e "projection" definidas acima e já enviadas
			// para a placa de vídeo (GPU).
			//
			// Veja a definição de g_VirtualScene["cube_faces"] dentro da
			// função BuildTriangles(), e veja a documentação da função
			// glDrawElements() em http://docs.gl/gl3/glDrawElements.
			glDrawElements(g_VirtualScene["cube_faces"]
			                   .rendering_mode, // Veja slides 182-188 do documento Aula_04_Modelagem_Geometrica_3D.pdf
			               g_VirtualScene["cube_faces"].num_indices, GL_UNSIGNED_INT,
			               (void *)g_VirtualScene["cube_faces"].first_index);

			// Pedimos para OpenGL desenhar linhas com largura de 4 pixels.
			glLineWidth(4.0f);

			// Pedimos para a GPU rasterizar os vértices dos eixos XYZ
			// apontados pelo VAO como linhas. Veja a definição de
			// g_VirtualScene["axes"] dentro da função BuildTriangles(), e veja
			// a documentação da função glDrawElements() em
			// http://docs.gl/gl3/glDrawElements.
			//
			// Importante: estes eixos serão desenhamos com a matriz "model"
			// definida acima, e portanto sofrerão as mesmas transformações
			// geométricas que o cubo. Isto é, estes eixos estarão
			// representando o sistema de coordenadas do modelo (e não o global)!
			glDrawElements(g_VirtualScene["axes"].rendering_mode, g_VirtualScene["axes"].num_indices, GL_UNSIGNED_INT,
			               (void *)g_VirtualScene["axes"].first_index);

			// Informamos para a placa de vídeo (GPU) que a variável booleana
			// "render_as_black" deve ser colocada como "true". Veja o arquivo
			// "shader_vertex.glsl".
			glUniform1i(render_as_black_uniform, true);

			// Pedimos para a GPU rasterizar os vértices do cubo apontados pelo
			// VAO como linhas, formando as arestas pretas do cubo. Veja a
			// definição de g_VirtualScene["cube_edges"] dentro da função
			// BuildTriangles(), e veja a documentação da função
			// glDrawElements() em http://docs.gl/gl3/glDrawElements.
			glDrawElements(g_VirtualScene["cube_edges"].rendering_mode, g_VirtualScene["cube_edges"].num_indices,
			               GL_UNSIGNED_INT, (void *)g_VirtualScene["cube_edges"].first_index);

			// Desenhamos um ponto de tamanho 15 pixels em cima do terceiro vértice
			// do terceiro cubo. Este vértice tem coordenada de modelo igual à
			// (0.5, 0.5, 0.5, 1.0).
			if (i == 3) {
				glPointSize(15.0f);
				glDrawArrays(GL_POINTS, 3, 1);
			}
		}

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

// ==================================================
// MARK: Keyboard callback
// ==================================================

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Se o usuário pressionou o botão esquerdo do mouse, guardamos a
		// posição atual do cursor nas variáveis g_LastCursorPosX e
		// g_LastCursorPosY.  Também, setamos a variável
		// g_LeftMouseButtonPressed como true, para saber que o usuário está
		// com o botão esquerdo pressionado.
		glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
		g_LeftMouseButtonPressed = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
		// variável abaixo para false.
		g_LeftMouseButtonPressed = false;
	}
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
	if (!g_LeftMouseButtonPressed)
		return;

	float dx = xpos - g_LastCursorPosX;
	float dy = ypos - g_LastCursorPosY;

	// Atualiza o ângulo de visão da câmera utilizando a movimentação do mouse.
	g_Camera.yaw -= dx * g_MouseSensitivity;
	g_Camera.pitch -= dy * g_MouseSensitivity;

	// Atualiza o view_vector da câmera utilizando os novos ângulos de visão.
	g_Camera.updateViewVector();

	g_LastCursorPosX = xpos;
	g_LastCursorPosY = ypos;
}

// Definição da função que será chamada sempre que o usuário pressionar alguma tecla do teclado.
// Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod) {

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_W:
			g_Camera.position += g_Camera.view_vector * g_MovementSpeed;
			break;
		case GLFW_KEY_S:
			g_Camera.position -= g_Camera.view_vector * g_MovementSpeed;
			break;
		case GLFW_KEY_A:
			g_Camera.position -= crossproduct(g_Camera.view_vector, g_Camera.up_vector) * g_MovementSpeed;
			break;
		case GLFW_KEY_D:
			g_Camera.position += crossproduct(g_Camera.view_vector, g_Camera.up_vector) * g_MovementSpeed;
			break;
		default:
			break;
		}
	}
}

// ==================================================
// MARK: Constroi os objetos do cenário
// FIX: Substituir por objetos do jogo, agora sao apenas placeholders
// ==================================================
GLuint BuildTriangles() {
	GLfloat model_coefficients[] = {
	    -0.5f, 0.5f, 0.5f,  1.0f, -0.5f, -0.5f, 0.5f,  1.0f,  0.5f,  -0.5f, 0.5f, 1.0f,  0.5f,  0.5f,
	    0.5f,  1.0f, -0.5f, 0.5f, -0.5f, 1.0f,  -0.5f, -0.5f, -0.5f, 1.0f,  0.5f, -0.5f, -0.5f, 1.0f,
	    0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
	    0.0f,  1.0f, 0.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
	};
	GLuint VBO_model_coefficients_id;
	glGenBuffers(1, &VBO_model_coefficients_id);
	GLuint vertex_array_object_id;
	glGenVertexArrays(1, &vertex_array_object_id);
	glBindVertexArray(vertex_array_object_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model_coefficients), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model_coefficients), model_coefficients);
	GLuint location = 0;
	GLint number_of_dimensions = 4;
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLfloat color_coefficients[] = {
	    1.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.5f,
	    1.0f, 1.0f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f,
	    0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};
	GLuint VBO_color_coefficients_id;
	glGenBuffers(1, &VBO_color_coefficients_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
	location = 1;
	number_of_dimensions = 4;
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint indices[] = {0, 1, 2, 7, 6, 5, 3, 2, 6, 4, 0, 3, 4, 5, 1, 1, 5, 6, 0,  2,  3,  7,
	                    5, 4, 3, 6, 7, 4, 3, 7, 4, 1, 0, 1, 6, 2, 0, 1, 1, 2, 2,  3,  3,  0,
	                    0, 4, 4, 7, 7, 6, 6, 2, 6, 5, 5, 4, 5, 1, 7, 3, 8, 9, 10, 11, 12, 13};
	SceneObject cube_faces;
	cube_faces.name = "Cubo (faces coloridas)";
	cube_faces.first_index = (void *)0;
	cube_faces.num_indices = 36;
	cube_faces.rendering_mode = GL_TRIANGLES;
	g_VirtualScene["cube_faces"] = cube_faces;
	SceneObject cube_edges;
	cube_edges.name = "Cubo (arestas pretas)";
	cube_edges.first_index = (void *)(36 * sizeof(GLuint));
	cube_edges.num_indices = 24;
	cube_edges.rendering_mode = GL_LINES;
	g_VirtualScene["cube_edges"] = cube_edges;
	SceneObject axes;
	axes.name = "Eixos XYZ";
	axes.first_index = (void *)(60 * sizeof(GLuint));
	axes.num_indices = 6;
	axes.rendering_mode = GL_LINES;
	g_VirtualScene["axes"] = axes;
	GLuint indices_id;
	glGenBuffers(1, &indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
	glBindVertexArray(0);
	return vertex_array_object_id;
}
