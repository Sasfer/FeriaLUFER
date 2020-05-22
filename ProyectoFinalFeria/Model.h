#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshTexturizado.h"
#include "Texture.h"

class Model{
	public:
		Model();

		void LoadModel(const std::string& fileName);
		void RenderModel();
		void ClearModel();

		~Model();

	private:
		// Assimp
		void LoadNode(aiNode* node, const aiScene* scene); 
		void LoadMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterials(const aiScene* scene);

		std::vector<Mesh*>MeshList;
		std::vector<Texture*>TextureList;
		std::vector<unsigned int>meshTotex;
};

