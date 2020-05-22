#include "Model.h"

Model::Model(){
}

void Model::LoadModel(const std::string & fileName){
	// Pasa de Polygons y Quads a triangulos, modifica orden para el origen, 
	// generar normales si el  objeto no tiene, trata vértices iguales como 1 solo
	Assimp::Importer importer;
	
	const aiScene *scene=importer.ReadFile(fileName,aiProcess_Triangulate |aiProcess_FlipUVs|
		aiProcess_GenSmoothNormals|aiProcess_JoinIdenticalVertices);
	
	if (!scene){	
		printf("Falló en cargar el modelo: %s \n", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::ClearModel(){
	for (size_t i = 0; i < MeshList.size(); i++){
		if (MeshList[i]){
			delete MeshList[i];
			MeshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < TextureList.size(); i++){
		if (TextureList[i])
		{
			delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}
}

void Model::RenderModel(){
	for (size_t i = 0; i < MeshList.size(); i++){
		unsigned int materialIndex = meshTotex[i];
		if (!materialIndex< TextureList.size()&& TextureList[materialIndex]){
			TextureList[materialIndex]->UseTexture();
		}
		MeshList[i]->RenderMesh();
	}
}

Model::~Model(){
}

void Model::LoadNode(aiNode * node, const aiScene * scene){
	for (size_t i = 0; i <node->mNumMeshes; i++){
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++){
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene){
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	
	for (size_t i = 0; i < mesh->mNumVertices; i++){
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z });
		
		// UV
		// Si tiene coordenadas de texturizado
		if (mesh->mTextureCoords[0]){
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
		}
		else{
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}

		// Normals importante, las normales son negativas porque la luz interactúa 
		// con ellas de esa forma, cómo se vio con el dado/cubo
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y ,-mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(newMesh);
	meshTotex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene){
	TextureList.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene ->mNumMaterials; i++){
		aiMaterial* material = scene->mMaterials[i];

		TextureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE	)){
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE,0,&path)==AI_SUCCESS){
				// Para quitar del path del modelo todo lo que este antes del \ de ubicación de directorio
				int idx = std::string(path.data).rfind("\\");
				
				std::string filename = std::string(path.data).substr(idx + 1);
				std::string texPath = std::string("textures/") + filename;
				
				TextureList[i] = new Texture(texPath.c_str());
				
				if (!TextureList[i]->LoadTexture()){
					printf("Falló en cargar la Textura :%s\n", texPath);
					delete TextureList[i];
					TextureList[i] = nullptr;
				}
			}
		}

		if (!TextureList[i]){
			// Textura que se aplicará a los modelos si no tienen textura o la textura no se puede cargar
			TextureList[i] = new Texture("textures/BaseBlanca.png"); 
			TextureList[i]->LoadTextureA();
		}
	}
}
