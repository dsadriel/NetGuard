#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 global_light_direction;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 normal;
out vec2 texture_coordinates;
out vec4 model_coordinates;
out vec4 vertex_color;

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

#define SHADING_PHONG   0 
#define SHADING_GOURAUD 1 
uniform int shading_mode;

void main()
{
    // A variável gl_Position define a posição final de cada vértice
    // OBRIGATORIAMENTE em "normalized device coordinates" (NDC), onde cada
    // coeficiente estará entre -1 e 1 após divisão por w.
    // Veja {+NDC2+}.
    //
    // O código em "main.cpp" define os vértices dos modelos em coordenadas
    // locais de cada modelo (array model_coefficients). Abaixo, utilizamos
    // operações de modelagem, definição da câmera, e projeção, para computar
    // as coordenadas finais em NDC (variável gl_Position). Após a execução
    // deste Vertex Shader, a placa de vídeo (GPU) fará a divisão por W. Veja
    // slides 41-67 e 69-86 do documento Aula_09_Projecoes.pdf.

    gl_Position = projection * view * model * model_coefficients;

    // Como as variáveis acima  (tipo vec4) são vetores com 4 coeficientes,
    // também é possível acessar e modificar cada coeficiente de maneira
    // independente. Esses são indexados pelos nomes x, y, z, e w (nessa
    // ordem, isto é, 'x' é o primeiro coeficiente, 'y' é o segundo, ...):
    //
    //     gl_Position.x = model_coefficients.x;
    //     gl_Position.y = model_coefficients.y;
    //     gl_Position.z = model_coefficients.z;
    //     gl_Position.w = model_coefficients.w;
    //

    texture_coordinates = texture_coefficients;
    model_coordinates = model_coefficients;

    // Agora definimos outros atributos dos vértices que serão interpolados pelo
    // rasterizador para gerar atributos únicos para cada fragmento gerado.

    // Posição do vértice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;

    // Normal do vértice atual no sistema de coordenadas global (World).
    // Veja slides 123-151 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;


    if(shading_mode == SHADING_GOURAUD)
    {
        vec4 p = position_world;
        vec4 origin=vec4(0.,0.,0.,1.);
        vec4 camera_position=inverse(view)*origin;
        
        // Normal do fragmento atual, interpolada pelo rasterizador a partir das
        // normais de cada vértice.
        vec4 n=normalize(normal);
        
        // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
        vec4 l=normalize(-global_light_direction);
        
        // Vetor que define o sentido da câmera em relação ao ponto atual.
        vec4 v=normalize(camera_position-position_world);
        
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
            float U = fract(model_coordinates.x / 2.0);
            float V = fract(model_coordinates.z / 2.0);
            
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
        vertex_color.a=1;
        
        vertex_color.rgb=lambert_diffuse_term+ambient_term+blinnPhong_specular_term;
        
        vertex_color.rgb=pow(vertex_color.rgb,vec3(1.,1.,1.)/2.2);
    }
    else if(shading_mode == SHADING_PHONG)
    {
        vertex_color = vec4(0.0, 0.0, 0.0, 1.0); 
    }
}

