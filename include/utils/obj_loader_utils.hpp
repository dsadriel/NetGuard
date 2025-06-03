#ifndef _OBJ_LOADER_UTILS_HPP
#define _OBJ_LOADER_UTILS_HPP

#include "core/scene.hpp"
#include "tinyObjLoader/tiny_obj_loader.h"
#include <iostream>

using namespace std;

// Estrutura que representa um modelo geométrico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .
struct ObjModel {
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;

	// Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
	// Veja: https://github.com/syoyo/tinyobjloader
	ObjModel(const char *filename, const char *basepath = NULL, bool triangulate = true) {
		printf("Carregando objetos do arquivo \"%s\"...\n", filename);

		// Se basepath == NULL, então setamos basepath como o dirname do
		// filename, para que os arquivos MTL sejam corretamente carregados caso
		// estejam no mesmo diretório dos arquivos OBJ.
		string fullpath(filename);
		string dirname;
		if (basepath == NULL) {
			auto i = fullpath.find_last_of("/");
			if (i != string::npos) {
				dirname = fullpath.substr(0, i + 1);
				basepath = dirname.c_str();
			}
		}

		string warn;
		string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, basepath, triangulate);

		if (!err.empty())
			fprintf(stderr, "\n%s\n", err.c_str());

		if (!ret)
			throw runtime_error("Erro ao carregar modelo.");

		for (size_t shape = 0; shape < shapes.size(); ++shape) {
			if (shapes[shape].name.empty()) {
				fprintf(stderr,
				        "*********************************************\n"
				        "Erro: Objeto sem nome dentro do arquivo '%s'.\n"
				        "Veja https://www.inf.ufrgs.br/~eslgastal/fcg-faq-etc.html#Modelos-3D-no-formato-OBJ .\n"
				        "*********************************************\n",
				        filename);
				throw runtime_error("Objeto sem nome.");
			}
			printf("- Objeto '%s'\n", shapes[shape].name.c_str());
		}

		printf("OK.\n");
	}
};

extern map<string, SceneObject> g_VirtualScene;

// Constrói triângulos para futura renderização a partir de um ObjModel.
void BuildTrianglesAndAddToVirtualScene(ObjModel *model);

// Função que computa as normais de um ObjModel, caso elas não tenham sido
// especificadas dentro do arquivo ".obj"
void ComputeNormals(ObjModel *model);

#endif