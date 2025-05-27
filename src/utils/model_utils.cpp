#include "utils/model_utils.h"
#include <core/scene.h>

// FIX: Substituir por objetos do jogo, agora sao apenas placeholders
// ==================================================
GLuint BuildTriangles() 
{
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
	cube_faces.first_index = 0;
	cube_faces.num_indices = 36;
	cube_faces.rendering_mode = GL_TRIANGLES;
	g_VirtualScene["cube_faces"] = cube_faces;
	SceneObject cube_edges;
	cube_edges.name = "Cubo (arestas pretas)";
	cube_edges.first_index = (36 * sizeof(GLuint));
	cube_edges.num_indices = 24;
	cube_edges.rendering_mode = GL_LINES;
	g_VirtualScene["cube_edges"] = cube_edges;
	SceneObject axes;
	axes.name = "Eixos XYZ";
	axes.first_index = (60 * sizeof(GLuint));
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