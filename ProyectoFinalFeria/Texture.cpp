#include "Texture.h"
#include "CommonValues.h"

Texture::Texture(){
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;
}

Texture::Texture(const char *FileLoc){
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = FileLoc;
}

bool Texture::LoadTextureA(){
	// Para cambiar el origen a la esquina inferior izquierda como necesitamos
	stbi_set_flip_vertically_on_load(true);
	// El tipo unsigned char es para un array de bytes de la imagen, obtener datos de la imagen 
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth,0); 
	
	if (!texData){
		printf("No se encontró el archivo: %s", fileLocation);
	}

	// Parecido al VAO: crear una textura y asignarle un índice
	glGenTextures(1, &textureID); 
	// Se indica que la textura es de tipo 2D, para superficies planas es suficiente esta textura
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	// Eje S paralelo a X, repetir sobre el eje
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// Eje S paralelo a X, repetir sobre el eje pero rotando con forme a un centro
/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	// Eje S paralelo a X, envolver toda la superficie
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
*/
	// Eje T paralelo a Y, repetir sobre el eje
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*
	GL_TEXTURE_MIN_FILTER: Para más cerca o textura se escala a menor tamaño. 
	GL_TEXTURE_MAG_FILTER: Para más lejos o textura se escala a mayor tamaño. 
	GL_LINEAR:  Aplica sampling y blending de texels más cercanos. 
	GL_NEAREST: Aplica sample de texel más cercano
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//if(GL_RGBA) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	/*}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	}*/

	glGenerateMipmap(GL_TEXTURE_2D); 
	// Para hacer un unbind de la textura
	glBindTexture(GL_TEXTURE_2D, 0);
	// Para liberar la información de la imagen
	stbi_image_free(texData); 

	return true;
}

bool Texture::LoadTexture(){
	// Para cambiar el origen a la esquina inferior izquierda como necesitamos
	stbi_set_flip_vertically_on_load(true);

	// El tipo unsigned char es para un array de bytes de la imagen, obtener datos de la imagen 
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); 

	if (!texData){
		printf("No se encontró el archivo: %s", fileLocation);
	}

	// Parecido al VAO: crear una textura y asignarle un índice
	glGenTextures(1, &textureID); 
	// Se indica que la textura es de tipo 2D, para superficies planas es suficiente esta textura
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Eje S paralelo a X, repetir sobre el eje
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// Eje S paralelo a X, repetir sobre el eje pero rotando con forme a un centro
/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	// Eje S paralelo a X, envolver toda la superficie
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
*/
	// Eje T paralelo a Y, repetir sobre el eje
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
	GL_TEXTURE_MIN_FILTER: Para más cerca o textura se escala a menor tamaño. 
	GL_TEXTURE_MAG_FILTER: Para más lejos o textura se escala a mayor tamaño.
	GL_LINEAR: Aplica sampling y blending de texels más cercanos. 
	GL_NEAREST: Aplica sample de texel más cercano
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Para hacer un unbind de la textura
	glBindTexture(GL_TEXTURE_2D, 0);
	// Para liberar la información de la imagen
	stbi_image_free(texData); 
	return true;
}

void Texture::ClearTexture(){

	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

void Texture::UseTexture(){	
	// UnitTexture
	// Para crear un sampler que es lo que necesitan los shaders para 
	// poder acceder a la textura: 16 a 32 texturas pueden ser declaradas
	glActiveTexture(GL_TEXTURE0); 
	
	// Si hay mas de 1 unittexture se tiene que crear una unifromvariable que haga cambio entre la unit texture a utilizar
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture(){
	ClearTexture();
}
