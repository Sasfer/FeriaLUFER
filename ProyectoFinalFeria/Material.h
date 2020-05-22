#pragma once

#include <glew.h>

// Clase para crear materiales asignables a los diferentes objetos y luces
class Material{
	public:
		Material();
		Material(GLfloat sIntensity, GLfloat shine);

		void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

		~Material();

	private: 
		GLfloat specularIntensity;
		GLfloat shininess;
};

