#version 330 core

in vec2 fragTexCoord;              // coordenadas vindas do vertex shader
out vec4 color;                    // cor final do pixel

uniform sampler2D TextureImage;    // textura carregada no C++

void main()
{
    color = texture(TextureImage, fragTexCoord);
}
