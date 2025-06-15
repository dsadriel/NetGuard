#ifndef _SCENE_HPP
#define _SCENE_HPP

#include <string>
#include <glad/glad.h> 
#include <glm/gtc/type_ptr.hpp>

#include "matrices.hpp"

using namespace std;

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject
{
    string       name;        // Nome do objeto
    size_t       first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo

    glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);


    void drawObject(GLint model_uniform, GLint object_id_uniform, int object_id)
    {
        // Construção da matriz Model
        glm::mat4 model = 
        Matrix_Identity()
        * Matrix_Translate(position.x, position.y, position.z)
        * Matrix_Rotate_X(rotation.x)
        * Matrix_Rotate_Y(rotation.y)
        * Matrix_Rotate_Z(rotation.z)
        * Matrix_Scale(scale.x, scale.y, scale.z);

        // Envio da matriz para o shader
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));

        // Envio do ID do objeto (para shader saber que objeto é)
        glUniform1i(object_id_uniform, object_id);

        // Renderização
        glBindVertexArray(vertex_array_object_id);
        glDrawElements(rendering_mode, num_indices, GL_UNSIGNED_INT, (void*)(first_index * sizeof(GLuint)));
        glBindVertexArray(0);
    }
};

#endif