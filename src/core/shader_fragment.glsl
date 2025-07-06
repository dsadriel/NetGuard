#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;
in vec2 texture_coordinates;
in vec4 model_coordinates;
in vec4 vertex_color;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual o estilo do objeto
#define PLAIN_COLOR 0x00
#define TEXTURED 0x10
#define MTL_TEXTURE 0x11
#define FLAT_TEXTURED 0x12
#define PLAIN_TEXTURED 0x13
uniform int object_style;

// Identificador que define a cor do objeto, caso seja o estilo de color
uniform vec4 object_color;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;

#define SHADING_PHONG 0 
#define SHADING_GOURAUD 1 
uniform int shading_mode;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

void main()
{

    if(object_style==PLAIN_TEXTURED) 
        {
            float U = texture_coordinates.x;
            float V = texture_coordinates.y;
            
            color = texture(TextureImage0, vec2(U, V));
            color.rgb = pow(color.rgb, vec3(1., 1., 1.) / 2.2);
            return; // Retorna imediatamente sem calcular iluminação
        } 
    
    if(shading_mode == SHADING_GOURAUD)
    {
        color.rgb = vertex_color.rgb;
        color.a=1;
        color.rgb=pow(color.rgb,vec3(1.,1.,1.)/2.2);
    }
    else
    {
        // Obtemos a posição da câmera utilizando a inversa da matriz que define o
        // sistema de coordenadas da câmera.
        vec4 origin=vec4(0.,0.,0.,1.);
        vec4 camera_position=inverse(view)*origin;
        
        // O fragmento atual é coberto por um ponto que percente à superfície de um
        // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
        // sistema de coordenadas global (World coordinates). Esta posição é obtida
        // através da interpolação, feita pelo rasterizador, da posição de cada
        // vértice.
        vec4 p=position_world;
        
        // Normal do fragmento atual, interpolada pelo rasterizador a partir das
        // normais de cada vértice.
        vec4 n=normalize(normal);
        
        // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
        vec4 l=normalize(camera_position-p);
        
        // Vetor que define o sentido da câmera em relação ao ponto atual.
        vec4 v=normalize(camera_position-p);
        
        // Vetor que define o sentido da reflexão especular ideal.
        vec4 r=-l+2.*n*dot(n,l);
        
        // Parâmetros que definem as propriedades espectrais da superfície
        vec3 Kd=vec3(.2,.2,.2);// Refletância difusa
        vec3 Ks=vec3(.3,.3,.3);;// Refletância especular
        vec3 Ka=vec3(0.,0.,0.);;// Refletância ambiente
        float q=20.;// Expoente especular para o modelo de iluminação de Phong
        
        if(object_style==MTL_TEXTURE)
        {
            // Coordenadas de textura U e V
            float U = texture_coordinates.x;
            float V = texture_coordinates.y;
            
            Kd=texture(TextureImage0,vec2(U,V)).rgb;
            Ka=Kd/2;
        } else if (object_style==FLAT_TEXTURED) {
            // Coordenadas de textura U e V
            float U = fract(model_coordinates.x);
            float V = fract(model_coordinates.z);
            
            Kd=texture(TextureImage0,vec2(U,V)).rgb;
            Ka=Kd/10;
        } else if(object_style==PLAIN_COLOR) {
            Kd=object_color.rgb;
            Ka=Kd/2;
        }

        
        // Espectro da fonte de iluminação
        vec3 I=vec3(1.,1.,1.);

        // Espectro da luz ambiente
        vec3 Ia=vec3(.2,.2,.2);
        
        // Termo difuso utilizando a lei dos cossenos de Lambert  (slide 131)
        vec3 lambert_diffuse_term=Kd*I*max(0,dot(n,l));
        
        // Termo ambiente  (slide 131)
        vec3 ambient_term=Ka*Ia;

        vec4 halfVector = normalize(v + l);
        vec3 blinnPhong_specular_term = Ks * I * pow(max(0, dot(n, halfVector)), q); //o termo especular de Blinn-Phong
        color.a=1;
        
        color.rgb=lambert_diffuse_term+ambient_term+blinnPhong_specular_term;
        
        color.rgb=pow(color.rgb,vec3(1.,1.,1.)/2.2);
    }
}