#pragma once


#include <vector>
#include <string>
#include <glew.h>
#include <glm.hpp>
#include <glfw3.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "CommonValues.h"
#include "MeshTexturizado.h"
#include "ShaderLight.h" 

class Skybox{
	public:
		Skybox();
		Skybox(std::vector<std::string> faceLocations);
		void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
		
		~Skybox();
	private:
		Mesh* skyMesh;
		Shader* skyShader;
		GLuint textureId;
		GLuint uniformProjection, uniformView;
};

