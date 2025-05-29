#version 330 core

layout(location = 0) in vec4 position;    // posição do vértice
layout(location = 1) in vec2 texcoord;    // coordenada de textura

out vec2 fragTexCoord;                    // para passar ao fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * position;
    fragTexCoord = texcoord;
}