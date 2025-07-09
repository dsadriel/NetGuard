#ifndef _SCENE_HPP
#define _SCENE_HPP

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <external/glad/glad.h>
#include <external/glm/gtc/type_ptr.hpp>
#include <string>

#include "matrices.hpp"
#include "utils/texture_utils.hpp"
#include "collisions.hpp"

#define PLAIN_COLOR 0x00
#define TEXTURED 0x10
#define MTL_TEXTURED 0x11
#define FLAT_TEXTURED 0x12
#define PLAIN_TEXTURED 0x13

#define SHADING_PHONG 0
#define SHADING_GOURAUD 1

using namespace std;
using namespace glm;

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject {
	string name;                   // Nome do objeto
	size_t first_index;            // Índice do primeiro vértice dentro do vetor indices[] definido em
	                               // BuildTrianglesAndAddToVirtualScene()
	size_t num_indices;            // Número de índices do objeto dentro do vetor indices[] definido em
	                               // BuildTrianglesAndAddToVirtualScene()
	GLenum rendering_mode;         // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
	GLuint vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
	GLuint texture_id = 0;             // ID da textura
	GLuint object_style = PLAIN_COLOR;

	int shading_mode = 0; // Por padrão PHONG SHADING == 0, GOURAUD SHADING == 1 
	vec4 color = vec4(.3f, .5f, .3f, 1.0f);

	vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	void drawObject(GLint model_uniform, GLint object_style_uniform, GLint object_color_uniform, GLint shading_mode_uniform) {
		// Construção da matriz Model
		glm::mat4 model = Matrix_Identity() * Matrix_Translate(position.x, position.y, position.z) *
		                  Matrix_Rotate_X(rotation.x) * Matrix_Rotate_Y(rotation.y) * Matrix_Rotate_Z(rotation.z) *
		                  Matrix_Scale(scale.x, scale.y, scale.z);

		// Envio da matriz para o shader
		glUniformMatrix4fv(model_uniform, 1, GL_FALSE, value_ptr(model));

		// Envio do ID do objeto (para shader saber que objeto é)
		glUniform1i(object_style_uniform, object_style);

        glUniform4fv(object_color_uniform, 1, value_ptr(color));

		glUniform1i(shading_mode_uniform, shading_mode);

		// Se o objeto tem textura, ativa a textura
		if ((object_style & TEXTURED) && texture_id != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}

		// Renderização
		glBindVertexArray(vertex_array_object_id);
		glDrawElements(rendering_mode, num_indices, GL_UNSIGNED_INT, (void *)(first_index * sizeof(GLuint)));
		glBindVertexArray(0);
	}

	void applyTexture(const char *filename) {
		texture_id = LoadTextureImage(filename);
		object_style = MTL_TEXTURED;
	}
};

#endif