#include "shader_loader.hpp"

void LoadShadersFromFiles()
{
    GLuint vertex_shader_id = LoadShader_Vertex("../../src/core/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/core/shader_fragment.glsl");

    // Deletamos o programa de GPU anterior, caso ele exista.
    if ( g_GpuProgramID != 0 )
        glDeleteProgram(g_GpuProgramID);

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    g_GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    g_model_uniform      = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
    g_view_uniform       = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    g_projection_uniform = glGetUniformLocation(g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
	g_shading_mode_uniform = glGetUniformLocation(g_GpuProgramID, "shading_mode");
	g_object_style_uniform = glGetUniformLocation(g_GpuProgramID, "object_style");
	g_object_color_uniform = glGetUniformLocation(g_GpuProgramID, "object_color");
	g_texture0_uniform = glGetUniformLocation(g_GpuProgramID, "TextureImage0");
	g_texture1_uniform = glGetUniformLocation(g_GpuProgramID, "TextureImage1");
	g_texture2_uniform = glGetUniformLocation(g_GpuProgramID, "TextureImage2");
	g_global_light_direction_uniform = glGetUniformLocation(g_GpuProgramID, "global_light_direction");

}

GLuint LoadShader_Vertex(const char *filename) {
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	LoadShader(filename, vertex_shader_id);

	return vertex_shader_id;
}

GLuint LoadShader_Fragment(const char *filename) {

	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	LoadShader(filename, fragment_shader_id);

	return fragment_shader_id;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void LoadShader(const char *filename, GLuint shader_id) {
	// Lemos o arquivo de texto indicado pela variável "filename"
	// e colocamos seu conteúdo em memória, apontado pela variável
	// "shader_string".
	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit);
		file.open(filename);
	} catch (std::exception &e) {
		fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
		std::exit(EXIT_FAILURE);
	}
	std::stringstream shader;
	shader << file.rdbuf();
	std::string str = shader.str();
	const GLchar *shader_string = str.c_str();
	const GLint shader_string_length = static_cast<GLint>(str.length());

	// Define o código do shader GLSL, contido na string "shader_string"
	glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

	// Compila o código do shader GLSL (em tempo de execução)
	glCompileShader(shader_id);

	// Verificamos se ocorreu algum erro ou "warning" durante a compilação
	GLint compiled_ok;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

	GLint log_length = 0;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

	// Alocamos memória para guardar o log de compilação.
	// A chamada "new" em C++ é equivalente ao "malloc()" do C.
	GLchar *log = new GLchar[log_length];
	glGetShaderInfoLog(shader_id, log_length, &log_length, log);

	// Imprime no terminal qualquer erro ou "warning" de compilação
	if (log_length != 0) {
		std::string output;

		if (!compiled_ok) {
			output += "ERROR: OpenGL compilation of \"";
			output += filename;
			output += "\" failed.\n";
			output += "== Start of compilation log\n";
			output += log;
			output += "== End of compilation log\n";
		} else {
			output += "WARNING: OpenGL compilation of \"";
			output += filename;
			output += "\".\n";
			output += "== Start of compilation log\n";
			output += log;
			output += "== End of compilation log\n";
		}

		fprintf(stderr, "%s", output.c_str());
	}

	// A chamada "delete" em C++ é equivalente ao "free()" do C
	delete[] log;
}


// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
	// Criamos um identificador (ID) para este programa de GPU
	GLuint program_id = glCreateProgram();

	// Definição dos dois shaders GLSL que devem ser executados pelo programa
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);

	// Linkagem dos shaders acima ao programa
	glLinkProgram(program_id);

	// Verificamos se ocorreu algum erro durante a linkagem
	GLint linked_ok = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

	// Imprime no terminal qualquer erro de linkagem
	if (linked_ok == GL_FALSE) {
		GLint log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

		// Alocamos memória para guardar o log de compilação.
		// A chamada "new" em C++ é equivalente ao "malloc()" do C.
		GLchar *log = new GLchar[log_length];

		glGetProgramInfoLog(program_id, log_length, &log_length, log);

		std::string output;

		output += "ERROR: OpenGL linking of program failed.\n";
		output += "== Start of link log\n";
		output += log;
		output += "\n== End of link log\n";

		// A chamada "delete" em C++ é equivalente ao "free()" do C
		delete[] log;

		fprintf(stderr, "%s", output.c_str());
	}

	// Retornamos o ID gerado acima
	return program_id;
}
