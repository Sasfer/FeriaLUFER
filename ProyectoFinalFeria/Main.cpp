/*
	******************** PROYECTO FINAL - FERIA ********************
	Alumnos:
		Garrido López Luis Enrique
		Ortiz Figueroa María Fernanda

	Asignatura:
		Computación Gráfica e Iteracción Humano Computadora

	Profesor:
		Ing. José Roque Román Guadarrama

	SEMESTRE 2019-2
*/

// Para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "MeshTexturizado.h"
#include "ShaderLight.h"
#include "Camera.h"
#include "Texture.h"

// Para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

// Para modelos y skybox
#include "Model.h"
#include "Skybox.h"
#include "SpotLight.h"

// Para el audio
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
Camera camera;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Declaración de variables para uso de texturas
Texture pisoGeneralT;

Texture cactus1T;
Texture cactus2T;
Texture cactus3T;
Texture cactus4T;
Texture cactus5T;
Texture cactus6T;
Texture cactus7T;
Texture cactus8T;
Texture cosaCafeGiraT;

// Declaración de variables para uso de materiales
Material materialBrillante;
Material materialOpaco;

// Declaración de variables para uso de luces
// Luz direccional
DirectionalLight mainLight;
// Varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
// Varias luces de tipo spotlight
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Declaración de variables para uso de modelos
Model ruedaFortunaBaseM;
Model ruedaFortunaEjeM;
Model ruedaFortunaEstructuraM;
Model ruedaFortunaAsiento1M;
Model ruedaFortunaAsiento2M;

Model caballoCarruselM;
Model avestruzCarruselM;
Model avestruzCarrusel2M;
Model carruselInferiorM;
Model carruselSuperiorM;
Model espejosCarruselM;

Model baseCanoaM;
Model centroCanoaM;

Model torreMaderaM;
Model enfermeriaM;
Model souvenirsM;
Model globoM;
Model carroHeladoM;
Model carroComidaM;
Model banoM;

Model bancaM;
Model puestoComidaM;
Model bardaM;
Model letreroM;
Model lamparaM;

Model juegoDestrezaM;
Model patos1M;
Model patos2M;
Model patos3M;
Model patos4M;

glm::vec3 camaraPosicion = glm::vec3(170.0f, 60.0f, 0.0f);
float camaraYaw = 180.0f;
float camaraPitch = -30.0f;

// Declaración de variable para uso de skybox
Skybox skybox;

// Declaración de variable para uso de tiempo
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

// Cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset){
	for (size_t i = 0; i < indiceCount; i += 3){
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++){
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() {
	unsigned int pisoGeneralIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat pisoGeneralVertices[] = {
		//   x     y       z        u       v     nx    ny    nz
		-10.0f,	0.0f, -10.0f,	0.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		10.0f,	0.0f, -10.0f,	100.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,	0.0f,	100.0f,	0.0f, -1.0f, 0.0f,
		10.0f,	0.0f,  10.0f,	100.0f,	100.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0,	1,	2,
		0,	2,	3,
		4,	5,	6,
		4,	6,	7,
		8,	9,	10,
		8,	10,	11,
		12,	13,	14,
		12,	14,	15
	};

	GLfloat vegetacionVertices[] = {
		// Plano de frente
		-0.5f,	-0.5f,	0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f,	-0.5f,	0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f,	0.5f,	0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-0.5f,	0.5f,	0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		// Plano de lado
		0.0f,	-0.5f,	-0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f,	-0.5f,	0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f,	0.5f,	0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.0f,	0.5f,	-0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		// Plano diagonal
		-0.5f,	-0.5f,	-0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f,	-0.5f,	0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f,	0.5f,	0.5f,		1.0f, 1.0f,		1.0f, 0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		// Plano diagonal
		0.5f,	-0.5f,	-0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.5f,	-0.5f,	0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.5f,	0.5f,	0.5f,		1.0f, 1.0f,		1.0f, 0.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	};

	// Mesh 0 suelo estacionamiento
	Mesh *sueloE = new Mesh();
	sueloE->CreateMesh(pisoGeneralVertices, pisoGeneralIndices, 32, 6);
	meshList.push_back(sueloE);
	
	// Mesh 1 vegetación
	Mesh *vegetacion = new Mesh();
	vegetacion->CreateMesh(vegetacionVertices, vegetacionIndices, 128, 24);
	meshList.push_back(vegetacion);
}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
	// 1280, 1024 or 1024, 768
	mainWindow = Window(1024, 768);
	mainWindow.Initialise();

	// Música
	ISoundEngine *engine1 = createIrrKlangDevice();
	ISoundEngine *engine2 = createIrrKlangDevice();
	ISoundEngine *engine3 = createIrrKlangDevice();
	ISoundEngine *engine4 = createIrrKlangDevice();

	ISound *desiertoMusic = engine1->play3D("audio/Desierto.mp3", vec3df(8, -4, 0), true, false, true);
	ISound *morningMusic = engine2->play3D("audio/Morning.mp3", vec3df(8, -4, 0), true, false, true);
	ISound *prismoMusic = engine3->play3D("audio/Prismo.mp3", vec3df(8, -4, 0), true, false, true);
	ISound *differentMusic = engine4->play3D("audio/Different.mp3", vec3df(8, -4, 0), true, false, true);

	// Variable para la transición del skybox
	GLfloat tiempoSkybox;
	GLfloat velocidadSkybox;

	// Variable para movimiento
	float moverRuedaFortuna;
	float moverOffsetRuedaFortuna;
	float moverRuedaFortunaAsiento;
	float moverOffsetRuedaFortunaAsiento;

	float moverCarrusel;
	float moverOffsetCarrusel;
	float moverArribaCarrusel1;
	float moverOffsetArribaCarrusel1;
	float moverArribaCarrusel2;
	float moverOffsetArribaCarrusel2;
	bool avanzaArribaCarrusel1;
	bool avanzaArribaCarrusel2;

	float contadorCanoa;
	float moverCanoa;
	float moverOffsetCanoa;
	float anguloCanoa;

	bool prenderApagarLES;

	float tipoColorLuzCarrusel;
	float contLuzCarrusel;
	float radioLuzCarrusel;
	float xPosLuzCarrusel;
	float zPosLuzCarrusel;

	float contLuzRueda;
	float contLuzLetrero;
	float contLuzCanoa;

	bool camaraSalvar;

	float xPosRecCam;
	float zPosRecCam;
	float radioRecCam;
	float anguloRecCam;
	float anguloVisRecCam;
	float anguloVisRecCamAux;

	float movDesZ1;
	float movDesZ2;
	float movDesY1;
	float movDesY2;
	float auxMovDesY1;
	float auxMovDesY2;
	float auxMovDesZ1;
	float auxMovDesZ2;
	float contMovDes;
	float velMovDes;

	CreateObjects();
	CreateShaders();

	pisoGeneralT = Texture("textures/Tierra.tga");
	pisoGeneralT.LoadTextureA();

	cactus1T = Texture("textures/Cactus1.png");
	cactus1T.LoadTextureA();
	cactus2T = Texture("textures/Cactus2.png");
	cactus2T.LoadTextureA();
	cactus3T = Texture("textures/Cactus3.png");
	cactus3T.LoadTextureA();
	cactus4T = Texture("textures/Cactus4.png");
	cactus4T.LoadTextureA();
	cactus5T = Texture("textures/Cactus5.png");
	cactus5T.LoadTextureA();
	cactus6T = Texture("textures/Cactus6.png");
	cactus6T.LoadTextureA();
	cosaCafeGiraT = Texture("textures/CosaCafeGira.png");
	cosaCafeGiraT.LoadTextureA();

	materialBrillante = Material(4.0f, 256);
	materialOpaco = Material(0.3f, 4);
	
	ruedaFortunaAsiento1M = Model();
	ruedaFortunaAsiento1M.LoadModel("models/RuedaFortuna/ruedaFortunaAsiento1.obj");
	ruedaFortunaAsiento2M = Model();
	ruedaFortunaAsiento2M.LoadModel("models/RuedaFortuna/ruedaFortunaAsiento2.obj");
	ruedaFortunaEjeM = Model();
	ruedaFortunaEjeM.LoadModel("models/RuedaFortuna/ruedaFortunaEje.obj");
	ruedaFortunaBaseM = Model();
	ruedaFortunaBaseM.LoadModel("models/RuedaFortuna/ruedaFortunaBase.obj");
	ruedaFortunaEstructuraM = Model();
	ruedaFortunaEstructuraM.LoadModel("models/RuedaFortuna/ruedaFortunaEstructura.obj");
	
	caballoCarruselM = Model();
	caballoCarruselM.LoadModel("models/Carrusel/caballoP.obj");
	avestruzCarruselM = Model();
	avestruzCarruselM.LoadModel("models/Carrusel/avestruzP.obj");
	avestruzCarrusel2M = Model();
	avestruzCarrusel2M.LoadModel("models/Carrusel/avestruzP2.obj");
	carruselInferiorM = Model();
	carruselInferiorM.LoadModel("models/Carrusel/carruselInferior.obj");
	carruselSuperiorM = Model();
	carruselSuperiorM.LoadModel("models/Carrusel/carruselSuperior.obj");
	espejosCarruselM = Model();
	espejosCarruselM.LoadModel("models/Carrusel/espejosJuntos.obj");

	baseCanoaM = Model();
	baseCanoaM.LoadModel("models/Canoa/baseCanoa.obj");
	centroCanoaM = Model();
	centroCanoaM.LoadModel("models/Canoa/centroCanoa.obj");
	
	bardaM = Model();
	bardaM.LoadModel("models/ObjetosGenerales/BardaExterior.obj");
	letreroM = Model();
	letreroM.LoadModel("models/ObjetosGenerales/Letrero.obj");
	lamparaM = Model();
	lamparaM.LoadModel("models/ObjetosGenerales/Lampara.obj");
	bancaM = Model();
	bancaM.LoadModel("models/ObjetosGenerales/Silla.obj");
	enfermeriaM = Model();
	enfermeriaM.LoadModel("models/ObjetosGenerales/Enfermeria.obj");
	torreMaderaM = Model();
	torreMaderaM.LoadModel("models/ObjetosGenerales/Torre.obj");
	souvenirsM = Model();
	souvenirsM.LoadModel("models/ObjetosGenerales/Souvenirs.obj");
	globoM = Model();
	globoM.LoadModel("models/ObjetosGenerales/Globo.obj");
	
	carroComidaM = Model();
	carroComidaM.LoadModel("models/ObjetosGenerales/CarroHotdog.obj");
	carroHeladoM = Model();
	carroHeladoM.LoadModel("models/ObjetosGenerales/CarroHelado.obj");
	banoM = Model();
	banoM.LoadModel("models/ObjetosGenerales/Bano.obj");
	
	juegoDestrezaM = Model();
	juegoDestrezaM.LoadModel("models/JuegoDestreza/JuegoDestreza.obj");
	patos1M = Model();
	patos1M.LoadModel("models/JuegoDestreza/Patos1.obj");
	patos2M = Model();
	patos2M.LoadModel("models/JuegoDestreza/Patos2.obj");
	patos3M = Model();
	patos3M.LoadModel("models/JuegoDestreza/Patos3.obj");
	patos4M = Model();
	patos4M.LoadModel("models/JuegoDestreza/Patos4.obj");

	prenderApagarLES = 1;

	// Luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);

	// Declaración de luces puntuales
	// Contador de luces puntuales
	unsigned int pointLightCount = 0;
	// Luz puntual 0 lampara entrada
	pointLights[0] = PointLight(1.0f, 1.0f, 0.5f,
		0.5f, 2.0f,
		46.8f, 11.8f, -41.3f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz puntual 1 lampara salida
	pointLights[1] = PointLight(1.0f, 1.0f, 0.5f,
		0.5f, 2.0f,
		46.8f, 11.8f, 41.3f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz puntual 2 lampara salida
	pointLights[2] = PointLight(0.0f, 0.8f, 0.8f,
		0.5f, 9.0f,
		65.0f, 23.8f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Declaración de primer luz fija
	unsigned int spotLightCount = 0;

	// Luz fija 0 Carrusel
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 0.0f,
		80.0f);
	spotLightCount++;

	// Luz fija 1 Carrusel
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 0.0f,
		80.0f);
	spotLightCount++;

	// Luz fija 2 Carrusel
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 0.0f,
		80.0f);
	spotLightCount++;

	// Luz fija 3 Rueda de la Fortuna
	// Se puede mover con X -> UJ   Y -> IK   Z -> OL
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.5f,
		0.0f, 2.0f,
		-28.0f, 100.0f, -66.0f,
		0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 0.0f,
		12.0f);
	spotLightCount++;

	// Luz fija 4 Rueda de la Fortuna
	// Se puede mover con X -> UJ   Y -> IK   Z -> OL
	spotLights[4] = SpotLight(1.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		-10.0f, 118.0f, -68.0f,
		0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	// Luz fija 5, Rueda de la Fortuna
	// Se puede mover con X -> UJ   Y -> IK   Z -> OL
	spotLights[5] = SpotLight(0.0f, 0.0f, 0.5f,
		1.0f, 2.0f,
		-50.0f, 118.0f, -68.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	// Luz fija 6, Souvenirs
	spotLights[6] = SpotLight(1.0f, 0.4f, 1.4f,
		0.0f, 2.0f,
		32.0f, 6.0f, 26.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	// Luz fija 7, Juego de destreza
	spotLights[7] = SpotLight(1.0f, 0.8f, 0.4f,
		0.0f, 2.0f,
		0.0f, 6.0f, 26.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	// Luz fija 8, Canoa
	spotLights[8] = SpotLight(0.6f, 0.6f, 0.4f,
		0.0f, 2.0f,
		-36.0f, 32.0f, 64.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	// Skybox desierto, transiciones día a noche
	std::vector<std::string> skyboxFaces;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(45.0f,
		(GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	tiempoSkybox = 0.0;
	velocidadSkybox = 0.5;

	moverRuedaFortuna = 0.0f;
	moverOffsetRuedaFortuna = 10.0f;
	moverRuedaFortunaAsiento = 0.0;

	moverCarrusel = 0.0f;
	moverOffsetCarrusel = 10.0f;
	moverArribaCarrusel1 = 0.0f;
	moverOffsetArribaCarrusel1 = 0.1f;
	moverArribaCarrusel2 = 0.0f;
	moverOffsetArribaCarrusel2 = 0.15f;
	avanzaArribaCarrusel1 = 1;
	avanzaArribaCarrusel2 = 1;

	radioLuzCarrusel = 12.0f;
	tipoColorLuzCarrusel = 0.0f;
	contLuzCarrusel = 0.0f;

	contLuzRueda = 0.0f;
	contLuzLetrero = 0.0f;
	contLuzCanoa = 0.0f;

	camaraSalvar = 0;
	radioRecCam = 25.0f;
	xPosRecCam = 0.0f;
	zPosRecCam = 0.0f;
	anguloRecCam = 0.0f;
	anguloVisRecCam = 180.0f;
	anguloVisRecCamAux = 0.0f;

	movDesZ1 = 0.0f;
	movDesZ2 = 0.0f;
	movDesY1 = 0.0f;
	movDesY2 = 0.0f;
	auxMovDesY1 = 0.0f;
	auxMovDesY2 = 1.0f;
	auxMovDesZ1 = 0.0f;
	auxMovDesZ2 = 1.0f;
	contMovDes = 0.0f;
	velMovDes = 0.3f;

	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// Movimiento Juego de Destreza
		if (mainWindow.getAnimacionDestreza() == 1) {
			if (contMovDes >= 0.0f && contMovDes <= 100.0f) {
				velMovDes = 0.3f;
			}
			else if (contMovDes > 100.0f && contMovDes <= 200.0f) {
				velMovDes = 0.4f;
			}
			else if (contMovDes > 200.0f && contMovDes <= 300.0f) {
				velMovDes = 0.5f;
			}
			else if (contMovDes > 300.0f && contMovDes <= 400.0f) {
				velMovDes = 0.6f;
			}
			else if (contMovDes > 400.0f && contMovDes <= 500.0f) {
				velMovDes = 0.8f;
			}
			else if (contMovDes > 500.0f && contMovDes <= 600.0f) {
				velMovDes = 1.0f;
			}
			else {
				contMovDes = 0.0f;;
			}
			contMovDes++;

			// Movimiento arriba
			if (auxMovDesY1 == 0.0f) {
				if (movDesY1 <= 0.2f) {
					movDesY1 += 0.05 * deltaTime;
				}
				else{
					auxMovDesY1 = 1.0f;
				}
			}
			else {
				if (movDesY1 >= -0.2f) {
					movDesY1 -= 0.05 * deltaTime;
				}
				else {
					auxMovDesY1 = 0.0f;
				}
			}

			// Movimiento abajo
			if (auxMovDesY2 == 0.0f) {
				if (movDesY2 < 0.4f) {
					movDesY2 += 0.05 * deltaTime;
				}
				else {
					auxMovDesY2 = 1.0f;
				}
			}
			else {
				if (movDesY2 >= -0.4f) {
					movDesY2 -= 0.05 * deltaTime;
				}
				else {
					auxMovDesY2 = 0.0f;
				}
			}

			// Movimiento derecha
			if (auxMovDesZ1 == 0.0f) {
				if (movDesZ1 <= 2.4f) {
					movDesZ1 += velMovDes * deltaTime;
				}
				else {
					auxMovDesZ1 = 1.0f;
				}
			}
			else {
				if (movDesZ1 >= -2.4f) {
					movDesZ1 -= velMovDes * deltaTime;
				}
				else {
					auxMovDesZ1 = 0.0f;
				}
			}
			
			// Movimiento izquierda
			if (auxMovDesZ2 == 0.0f) {
				if (movDesZ2 <= 2.4f) {
					movDesZ2 += velMovDes * deltaTime;
				}
				else {
					auxMovDesZ2 = 1.0f;
				}
			}
			else {
				if (movDesZ2 >= -2.4f) {
					movDesZ2 -= velMovDes * deltaTime;
				}
				else {
					auxMovDesZ2 = 0.0f;
				}
			}
		}

		// Movimiento Rueda de la Fortuna
		if (mainWindow.getAnimacionRueda() == 1) {
			// Movimiento general de la rueda de la fortuna
			if (moverRuedaFortuna < 360.0f) {
				moverRuedaFortuna += moverOffsetRuedaFortuna * deltaTime;
				moverRuedaFortunaAsiento -= moverOffsetRuedaFortuna * deltaTime;
			}
			else {
				moverRuedaFortuna = 0.0f;
				moverRuedaFortunaAsiento = 0.0f;
			}
		}

		// Movimiento Carrusel
		if (mainWindow.getAnimacionCarrusel() == 1) {
			// Movimiento general del carrusel
			if (moverCarrusel < 360.0f) {
				moverCarrusel += moverOffsetCarrusel * deltaTime;
			}
			else {
				moverCarrusel = 0.0f;
			}

			// Movimiento caballo y avestruz arriba 1
			if (avanzaArribaCarrusel1 == 1) {
				if (moverArribaCarrusel1 < 0.8f) {
					moverArribaCarrusel1 += moverOffsetArribaCarrusel1 * deltaTime;
				}
				else {
					avanzaArribaCarrusel1 = 0;
				}
			}
			else if (avanzaArribaCarrusel1 == 0) {
				if (moverArribaCarrusel1 > 0.0f) {
					moverArribaCarrusel1 -= moverOffsetArribaCarrusel1 * deltaTime;
				}
				else {
					avanzaArribaCarrusel1 = 1;
				}
			}

			// Movimiento caballo y avestruz arriba 2
			if (avanzaArribaCarrusel2 == 1) {
				if (moverArribaCarrusel2 < 0.8f) {
					moverArribaCarrusel2 += moverOffsetArribaCarrusel2 * deltaTime;
				}
				else {
					avanzaArribaCarrusel2 = 0;
				}
			}
			else if (avanzaArribaCarrusel2 == 0) {
				if (moverArribaCarrusel2 > 0.0f) {
					moverArribaCarrusel2 -= moverOffsetArribaCarrusel2 * deltaTime;
				}
				else {
					avanzaArribaCarrusel2 = 1;
				}
			}
		}
	
		// Transión del Skybox
		// En caso de querer modificar la velocidad
		// cambiar el valor de la variable velocidadSkybox
		if (tiempoSkybox == (velocidadSkybox * 0.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_0_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_0_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_0_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_0_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_0_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_0_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 100.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_1_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_1_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_1_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_1_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_1_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_1_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 200.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_2_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_2_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_2_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_2_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_2_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_2_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 300.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_3_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_3_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_3_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_3_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_3_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_3_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 400.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_4_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 500.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_4_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_4_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 600.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_6_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_6_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_6_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_6_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_6_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_6_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 700.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_7_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_7_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_7_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_7_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_7_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_7_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 0;
		}
		else if (tiempoSkybox == (velocidadSkybox * 800.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_8_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_8_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_8_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_8_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_8_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_8_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 0;
		}
		else if (tiempoSkybox == (velocidadSkybox * 900.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_9_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_9_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_9_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_9_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_9_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_9_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 0;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1000.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_10_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_10_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_10_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_10_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_10_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_10_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 0;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1100.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_11_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_11_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_11_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_11_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_11_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_11_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1200.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_12_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_12_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_12_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_12_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_12_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_12_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1300.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_13_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_13_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_13_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_13_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_13_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_13_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1400.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_14_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_14_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_14_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_14_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_14_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_14_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1500.0)) {
			skyboxFaces.clear();
			skyboxFaces.push_back("textures/skybox/arrakisday_15_rt.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_15_lf.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_15_dn.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_15_up.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_15_bk.tga");
			skyboxFaces.push_back("textures/skybox/arrakisday_15_ft.tga");
			skybox = Skybox(skyboxFaces);
			prenderApagarLES = 1;
		}
		else if (tiempoSkybox == (velocidadSkybox * 1600.0)) {
			tiempoSkybox = -1.0;
		}

		tiempoSkybox = tiempoSkybox + 1.0;

		//Recibir eventos del usuario
		glfwPollEvents();

		// Camara recorrido
		if (mainWindow.getRecCam() == 1) {
			// Se salva la posición de la cámara en primera persona
			if (mainWindow.cambioCamara == 0.0f && camaraSalvar == 1) {
				camaraPosicion = camera.getCameraPosition();
				camaraYaw = camera.getCameraYaw();
				camaraPitch = camera.getCameraPitch();
				camaraSalvar = 0;
			}

			// Ecuación paramétrica de la circunferencia
			// para hacer que rote la camara
			// NOTA: debe estar moviendose el carrusel para que funcione la camara
			zPosRecCam = 0.0f + (radioRecCam * cos((anguloRecCam)* toRadians));
			xPosRecCam = -10.0f + (radioRecCam * sin((anguloRecCam)* toRadians));

			// Variación del angulo para que rote la camara
			if (anguloRecCam <= 360.0f) {
				anguloRecCam = anguloRecCam + 0.2f;
			}
			else {
				anguloRecCam = 0.0f;
			}

			if (anguloRecCam <= 180.0f) {
				anguloVisRecCam = 180.0f;
			}
			else {
				anguloVisRecCam = 0.0f;
			}

			camera = Camera(glm::vec3(xPosRecCam, 10.0f, zPosRecCam), glm::vec3(0.0f, 1.0f, 0.0f), anguloVisRecCam, -10.0f, 5.0f, 0.5f);
			mainWindow.cambioCamara = 1.0f;
		}
		else {
			// La primera ves que se ejecuta la vista de la camara se coloca
		// de frente a la feria, es decir, en la entrada
			if (mainWindow.getCamaraGeneral() == 0.0f) {
				camera = Camera(camaraPosicion, glm::vec3(0.0f, 1.0f, 0.0f), camaraYaw, camaraPitch, 5.0f, 0.5f);
				mainWindow.cambioCamara = 1.0f;
				mainWindow.setCamara(1.0f);
			}
			// Solo la camara en primera persona se mueve
			else if (mainWindow.getCamaraGeneral() == 1.0f) {
				// Al regresar a la camara el primera persona, se regresa a la posición en la que estaba
				if (mainWindow.cambioCamara == 0.0f) {
					camera = Camera(camaraPosicion, glm::vec3(0.0f, 1.0f, 0.0f), camaraYaw, camaraPitch, 5.0f, 0.5f);
					mainWindow.cambioCamara = 1.0f;
					camaraSalvar = 1;
				}

				// Movimiento general de la camara
				camera.keyControl(mainWindow.getsKeys(), deltaTime * 5);
				camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			}
			// Se posiciona la camara en vista Carrusel
			else if (mainWindow.getCamaraGeneral() == 2.0f && mainWindow.cambioCamara == 0.0f) {
				// Se salva la posición de la cámara en primera persona
				if (mainWindow.cambioCamara == 0.0f && camaraSalvar == 1) {
					camaraPosicion = camera.getCameraPosition();
					camaraYaw = camera.getCameraYaw();
					camaraPitch = camera.getCameraPitch();
					camaraSalvar = 0;
				}

				camera = Camera(glm::vec3(40.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, -10.0f, 5.0f, 0.5f);
				mainWindow.cambioCamara = 1.0f;
			}
			// Se posiciona la camara en vista Rueda de la Fortuna
			else if (mainWindow.getCamaraGeneral() == 3.0f && mainWindow.cambioCamara == 0.0f) {
				// Se salva la posición de la cámara en primera persona
				if (mainWindow.cambioCamara == 0.0f && camaraSalvar == 1) {
					camaraPosicion = camera.getCameraPosition();
					camaraYaw = camera.getCameraYaw();
					camaraPitch = camera.getCameraPitch();
					camaraSalvar = 0;
				}

				camera = Camera(glm::vec3(-30.0f, 40.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -10.0f, 5.0f, 0.5f);
				mainWindow.cambioCamara = 1.0f;
			}
			// Se posiciona la camara en vista Tiendas
			else if (mainWindow.getCamaraGeneral() == 4.0f && mainWindow.cambioCamara == 0.0f) {
				// Se salva la posición de la cámara en primera persona
				if (mainWindow.cambioCamara == 0.0f && camaraSalvar == 1) {
					camaraPosicion = camera.getCameraPosition();
					camaraYaw = camera.getCameraYaw();
					camaraPitch = camera.getCameraPitch();
					camaraSalvar = 0;
				}
				camera = Camera(glm::vec3(15.0f, 15.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -10.0f, 5.0f, 0.5f);
				mainWindow.cambioCamara = 1.0f;
			}
			// Se posiciona la camara en vista aerea
			else if (mainWindow.getCamaraGeneral() == 5.0f && mainWindow.cambioCamara == 0.0f) {
				if (mainWindow.cambioCamara == 0.0f && camaraSalvar == 1) {
					camaraPosicion = camera.getCameraPosition();
					camaraYaw = camera.getCameraYaw();
					camaraPitch = camera.getCameraPitch();
					camaraSalvar = 0;
				}
				camera = Camera(glm::vec3(-20.0f, 180.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 5.0f, 0.5f);
				mainWindow.cambioCamara = 1.0f;
			}
		}

		// Reproducción de la música según la posición de la cámara
		// Fuera de la feria
		if (camera.getCameraPosition().x > 80 && camera.getCameraPosition().x <= 200) {
			desiertoMusic->setIsPaused(false);
			prismoMusic->setIsPaused(true);
			differentMusic->setIsPaused(true);
			morningMusic->setIsPaused(true);
		}
		// General
		else if (camera.getCameraPosition().x > 40 && camera.getCameraPosition().x <= 80) {

			desiertoMusic->setIsPaused(true);
			prismoMusic->setIsPaused(true);
			differentMusic->setIsPaused(true);
			morningMusic->setIsPaused(false);
		}
		// Carrusel
		else if (camera.getCameraPosition().x >= 0 && camera.getCameraPosition().x <= 40) {
			desiertoMusic->setIsPaused(true);
			prismoMusic->setIsPaused(false);
			differentMusic->setIsPaused(true);
			morningMusic->setIsPaused(true);
		}
		// Rueda
		else if (camera.getCameraPosition().x > -60 && camera.getCameraPosition().x < 0) {
			desiertoMusic->setIsPaused(true);
			prismoMusic->setIsPaused(true);
			differentMusic->setIsPaused(false);
			morningMusic->setIsPaused(true);
		}
		// General
		else if (camera.getCameraPosition().x >= -100 && camera.getCameraPosition().x <= -60) {
			desiertoMusic->setIsPaused(true);
			prismoMusic->setIsPaused(true);
			differentMusic->setIsPaused(true);
			morningMusic->setIsPaused(false);
		}
		// Fuera de la feria
		else if (camera.getCameraPosition().x >= -200 && camera.getCameraPosition().x < -100) {
			desiertoMusic->setIsPaused(false);
			prismoMusic->setIsPaused(true);
			differentMusic->setIsPaused(true);
			morningMusic->setIsPaused(true);
		}
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();

		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		// Model rueda fortuna
		glm::mat4 modelRF(1.0);
		glm::mat4 modelRF1(1.0);
		glm::mat4 modelRF2(1.0);
		// Model carrusel
		glm::mat4 modelC(1.0);
		glm::mat4 modelC1(1.0);
		glm::mat4 modelC2(1.0);
		// Model canoa
		glm::mat4 modelA(1.0);
		glm::mat4 modelA1(1.0);
		// Model juego destreza
		glm::mat4 modelJ(1.0);
		glm::mat4 modelJ1(1.0);
		
		// Canoa*********************************************************************************************
		// Base++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelA = glm::mat4(1.0);
		modelA = glm::translate(modelA, glm::vec3(-40.0f, -2.0f, 65.0f));
		modelA = glm::rotate(modelA, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelA1 = modelA;
		modelA = glm::scale(modelA, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelA));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		baseCanoaM.RenderModel();

		// Centro++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelA = modelA1;
		modelA = glm::scale(modelA, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelA));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		centroCanoaM.RenderModel();
		
		if (contLuzCanoa >= 0.0f && contLuzCanoa < 100.0f) {
			spotLights[8].SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
		}
		else if (contLuzCanoa >= 100.0f && contLuzCanoa < 200.0f) {
			spotLights[8].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
		}
		else if (contLuzCanoa >= 100.0f && contLuzCanoa < 200.0f) {
			spotLights[8].SetColor(glm::vec3(0.0f, 0.0f, 0.8f));
		}
		else if (contLuzCanoa >= 100.0f && contLuzCanoa < 200.0f) {
			spotLights[8].SetColor(glm::vec3(0.0f, 0.5f, 0.0f));
		}
		else{
			contLuzCanoa = -1.0f;
		}
		contLuzCanoa++;

		// Juego de destreza******************************************************************************
		modelJ = glm::mat4(1.0); 
		modelJ = glm::translate(modelJ, glm::vec3(2.0f, -2.0f, 70.0f));
		modelJ = glm::scale(modelJ, glm::vec3(1.0f, 1.0f, 1.0f));
		modelJ = glm::rotate(modelJ, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelJ));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		juegoDestrezaM.RenderModel();

		modelJ1 = modelJ;
		modelJ = glm::translate(modelJ, glm::vec3(0.0f, 0.0f + movDesY1, 0.0f + movDesZ1));
		modelJ = glm::scale(modelJ, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelJ));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		patos1M.RenderModel();

		modelJ = modelJ1;
		modelJ = glm::translate(modelJ, glm::vec3(0.0f, 0.0f + movDesY2, 0.0f + movDesZ1));
		modelJ = glm::scale(modelJ, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelJ));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		patos2M.RenderModel();

		modelJ = modelJ1;
		modelJ = glm::translate(modelJ, glm::vec3(0.0f, 0.0f + movDesY2, 0.0f + movDesZ2));
		modelJ = glm::scale(modelJ, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelJ));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		patos3M.RenderModel();

		modelJ = modelJ1;
		modelJ = glm::translate(modelJ, glm::vec3(0.0f, 0.0f + movDesY1, 0.0f + movDesZ2));
		modelJ = glm::scale(modelJ, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelJ));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		patos4M.RenderModel();

		// Carrusel******************************************************************************************
		// Parte superior++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = glm::mat4(1.0);
		modelC = glm::translate(modelC, glm::vec3(-10.0f, -2.0f, 0.0f));
		modelC2 = modelC;
		modelC = glm::scale(modelC, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		carruselInferiorM.RenderModel();

		// Parte inferior++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC2;
		modelC = glm::translate(modelC, glm::vec3(0.0f, -0.15f, 0.0f));
		modelC = glm::rotate(modelC, moverCarrusel * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelC2 = modelC;
		modelC = glm::scale(modelC, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		carruselSuperiorM.RenderModel();

		// Espejos parte superior++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC2;
		modelC2 = modelC;
		modelC = glm::scale(modelC, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		espejosCarruselM.RenderModel();

		// Nivel interior
		// Palo 1++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC1 = modelC;
		modelC = glm::translate(modelC, glm::vec3(4.9f, 3.5f + moverArribaCarrusel2, -0.4f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 2++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(0.0f, 2.0f + +moverArribaCarrusel1, -3.7f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarruselM.RenderModel();

		// Palo 3++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-4.9f, 3.5f + moverArribaCarrusel2, 0.4f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 4++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(0.0f, 2.0f + moverArribaCarrusel1, 3.7f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarruselM.RenderModel();

		// Nivel medio+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Palo 5++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(7.0f, 3.5f + moverArribaCarrusel2, 0.5f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 79.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 6++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(5.1f, 1.6f + moverArribaCarrusel1, -3.5f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC,-128.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarruselM.RenderModel();

		// Palo 7++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(0.0f, 3.5f + moverArribaCarrusel2, -6.9f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 176.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 8++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-4.4f, 1.6f + moverArribaCarrusel1, -4.2f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -33.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarruselM.RenderModel();

		// Palo 9++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-7.0f, 3.5f + moverArribaCarrusel2, 0.5f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -79.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 10++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-4.4f, 1.6f + moverArribaCarrusel1, 4.2f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 33.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarruselM.RenderModel();

		// Palo 11++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(0.0f, 3.5f + moverArribaCarrusel2, 6.9f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 12++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(3.9f, 1.6f + moverArribaCarrusel1, 4.3f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 128.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarruselM.RenderModel();

		// Nivel exterior+++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Palo 13++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(8.8f, 1.5f + moverArribaCarrusel1, 1.0f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 166.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarrusel2M.RenderModel();

		// Palo 14++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(9.0f, 2.8f + moverArribaCarrusel2, -4.3f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 117.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 15++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(5.7f, 2.8f + moverArribaCarrusel2, -8.4f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 143.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 16++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(1.0f, 1.5f + moverArribaCarrusel1, -8.7f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -95.0f* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarrusel2M.RenderModel();

		// Palo 17++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-4.6f, 2.8f + moverArribaCarrusel2, -8.9f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 198.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 18++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-8.7f, 2.8f + moverArribaCarrusel2, -4.6f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -112.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 19++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-8.8f, 1.5f + moverArribaCarrusel1, 1.0f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarrusel2M.RenderModel();

		// Palo 20++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-7.6f, 2.8f + moverArribaCarrusel2, 6.6f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -42.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 21++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(-3.8f, 2.8f + moverArribaCarrusel2, 9.3f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, -18.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 22++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(1.0f, 1.5f + moverArribaCarrusel1, 8.7f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 95.0f* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avestruzCarrusel2M.RenderModel();

		// Palo 23++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(6.1f, 2.8f + moverArribaCarrusel2, 7.9f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 40.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();

		// Palo 24++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelC = modelC1;
		modelC = glm::translate(modelC, glm::vec3(8.8f, 2.8f + moverArribaCarrusel2, 4.6f));
		modelC = glm::scale(modelC, glm::vec3(0.2f, 0.2f, 0.2f));
		modelC = glm::rotate(modelC, 61.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballoCarruselM.RenderModel();
		
		// Luz carrusel
		// Este conjunto de 3 luces rotatorias tambien pueden desplazarse
		// Se tiene un contador para el cambio de luces
		contLuzCarrusel++;

		// Ecuación paramétrica de la circunferencia
		// para hacer que roten las luces
		zPosLuzCarrusel = 0.0f + (radioLuzCarrusel * cos((moverCarrusel + 0.0f) * toRadians));
		xPosLuzCarrusel = -10.0f + (radioLuzCarrusel * sin((moverCarrusel + 0.0f) * toRadians));
		spotLights[0].SetPos(glm::vec3(xPosLuzCarrusel + mainWindow.getMovLuzSpotX(), 
										8.0f + mainWindow.getMovLuzSpotY(), 
										zPosLuzCarrusel + mainWindow.getMovLuzSpotZ()));

		zPosLuzCarrusel = 0.0f + (radioLuzCarrusel * cos((moverCarrusel + 120.0f) * toRadians));
		xPosLuzCarrusel = -10.0f + (radioLuzCarrusel * sin((moverCarrusel + 120.0f) * toRadians));
		spotLights[1].SetPos(glm::vec3(xPosLuzCarrusel + mainWindow.getMovLuzSpotX(), 
										8.0f + mainWindow.getMovLuzSpotY(), 
										zPosLuzCarrusel + mainWindow.getMovLuzSpotZ()));

		zPosLuzCarrusel = 0.0f + (radioLuzCarrusel * cos((moverCarrusel + 240.0f) * toRadians));
		xPosLuzCarrusel = -10.0f + (radioLuzCarrusel * sin((moverCarrusel + 240.0f) * toRadians));
		spotLights[2].SetPos(glm::vec3(xPosLuzCarrusel + mainWindow.getMovLuzSpotX(), 
										8.0f + mainWindow.getMovLuzSpotY(), 
										zPosLuzCarrusel + mainWindow.getMovLuzSpotZ()));

		// Luces carrusel encendidas
		if (mainWindow.getLuzCarrusel() == 1) {
			if (tipoColorLuzCarrusel == 0.0f) {
				spotLights[0].SetColor(glm::vec3(0.8f, 0.8f, 0.8f)); //Blanco
				spotLights[1].SetColor(glm::vec3(0.5f, 0.0f, 0.0f)); //Rojo
				spotLights[2].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); //Cyan
			}
			else if (tipoColorLuzCarrusel == 1.0f) {
				spotLights[0].SetColor(glm::vec3(1.0f, 0.0f, 1.0f)); //Magenta
				spotLights[1].SetColor(glm::vec3(0.0f, 0.5f, 0.0f)); //Verde
				spotLights[2].SetColor(glm::vec3(0.8f, 0.8f, 0.0f)); //Amarillo
			}
			else if (tipoColorLuzCarrusel == 2.0f) {
				spotLights[0].SetColor(glm::vec3(0.0f, 0.0f, 1.0f)); //Azul
				spotLights[1].SetColor(glm::vec3(0.8f, 0.8f, 0.8f)); //Blanco
				spotLights[2].SetColor(glm::vec3(0.5f, 0.0f, 0.0f)); //Rojo
			}
			else if (tipoColorLuzCarrusel == 4.0f) {
				spotLights[0].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); //Cyan
				spotLights[1].SetColor(glm::vec3(1.0f, 0.0f, 1.0f)); //Magenta
				spotLights[2].SetColor(glm::vec3(0.0f, 0.5f, 0.0f)); //Verde
			}
			else if (tipoColorLuzCarrusel == 5.0f) {
				spotLights[0].SetColor(glm::vec3(0.8f, 0.8f, 0.0f)); //Amarillo
				spotLights[1].SetColor(glm::vec3(0.0f, 0.0f, 1.0f)); //Azul
				spotLights[2].SetColor(glm::vec3(0.8f, 0.8f, 0.8f)); //Blanco
			}
			else if (tipoColorLuzCarrusel == 6.0f) {
				spotLights[0].SetColor(glm::vec3(0.5f, 0.0f, 0.0f)); //Rojo
				spotLights[1].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); //Cyan
				spotLights[2].SetColor(glm::vec3(1.0f, 0.0f, 1.0f)); //Magenta
			}
			else if (tipoColorLuzCarrusel == 7.0f) {
				spotLights[0].SetColor(glm::vec3(0.0f, 0.5f, 0.0f)); //Verde
				spotLights[1].SetColor(glm::vec3(0.8f, 0.8f, 0.0f)); //Amarillo
				spotLights[2].SetColor(glm::vec3(0.0f, 0.0f, 1.0f)); //Azul
			}

			if (contLuzCarrusel == 100.0) {
				tipoColorLuzCarrusel = 1.0f;
			}
			else if (contLuzCarrusel == 150.0) {
				tipoColorLuzCarrusel = 2.0f;
			}
			else if (contLuzCarrusel == 200.0) {
				tipoColorLuzCarrusel = 3.0f;
			}
			else if (contLuzCarrusel == 250.0) {
				tipoColorLuzCarrusel = 4.0f;
			}
			else if (contLuzCarrusel == 300.0) {
				tipoColorLuzCarrusel = 5.0f;
			}
			else if (contLuzCarrusel == 350.0) {
				tipoColorLuzCarrusel = 6.0f;
			}
			else if (contLuzCarrusel == 400.0) {
				tipoColorLuzCarrusel = 7.0f;
			}
			else if (contLuzCarrusel == 450.0) {
				tipoColorLuzCarrusel = 0.0f;
				contLuzCarrusel = 0.0f;
			}
		}
		// Luces carrusel apagadas
		else {
			spotLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f)); 
			spotLights[1].SetColor(glm::vec3(0.0f, 0.0f, 0.0f)); 
			spotLights[2].SetColor(glm::vec3(0.0f, 0.0f, 0.0f)); 
		}
		
		// Rueda de la fortuna*******************************************************************************
		// Base
		modelRF = glm::mat4(1.0);
		modelRF = glm::translate(modelRF, glm::vec3(-30.0f, -2.0f, -68.0f));
		modelRF = glm::rotate(modelRF, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelRF1 = modelRF;
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaFortunaBaseM.RenderModel();

		// Eje
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF, glm::vec3(0.0f, 42.0f, 0.0f));
		modelRF = glm::rotate(modelRF, moverRuedaFortuna * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelRF1 = modelRF;
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaFortunaEjeM.RenderModel();
		
		// Estructura
		modelRF = modelRF1;
		modelRF1 = modelRF;
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaFortunaEstructuraM.RenderModel();
		
		// AsientoTipo 1
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF, glm::vec3(0.0f, -28.0f, 0.0f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento1M.RenderModel();

		// AsientoTipo 2
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, -25.0f, 14.6f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento2M.RenderModel();

		// AsientoTipo 3
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, -14.0f, 25.0f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento1M.RenderModel();

		// AsientoTipo 4
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 0.2f, 28.8f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento2M.RenderModel();

		// AsientoTipo 5
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 14.8f, 25.2f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento1M.RenderModel();

		// AsientoTipo 6
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 24.8f, 14.2f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento2M.RenderModel();
		
		// AsientoTipo 7
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 29.0f, 0.0f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento1M.RenderModel();

		// AsientoTipo 8
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 24.8f, -14.2f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento2M.RenderModel();

		// AsientoTipo 9
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 14.8f, -25.2f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento1M.RenderModel();

		// AsientoTipo 10
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, 0.2f, -28.8f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento2M.RenderModel();

		// AsientoTipo 11
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, -14.0f, -25.0f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento1M.RenderModel();

		// AsientoTipo 12
		modelRF = modelRF1;
		modelRF = glm::translate(modelRF1, glm::vec3(0.0f, -25.0f, -14.6f));
		modelRF = glm::scale(modelRF, glm::vec3(1.2f, 1.2f, 1.2f));
		modelRF = glm::rotate(modelRF, moverRuedaFortunaAsiento * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRF));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);
		ruedaFortunaAsiento2M.RenderModel();
		
		// Luz Rueda de la Fortuna
		// Luces encendidas
		if (mainWindow.getLuzRueda() == 1) {
			if (contLuzRueda >= 0 && contLuzRueda < 100.0f) {
				spotLights[3].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
				spotLights[4].SetColor(glm::vec3(1.0f, 1.0f, 0.5f));
				spotLights[5].SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
			}
			else if (contLuzRueda >= 100 && contLuzRueda < 200.0f) {
				spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
				spotLights[4].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
				spotLights[5].SetColor(glm::vec3(1.0f, 1.0f, 0.5f));
			}
			else if (contLuzRueda >= 200 && contLuzRueda < 300.0f) {
				spotLights[3].SetColor(glm::vec3(1.0f, 1.0f, 0.5f));
				spotLights[4].SetColor(glm::vec3(0.0f, 0.0f, 0.5f));
				spotLights[5].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
			}
			else {
				contLuzRueda = -1.0f;
			}
			contLuzRueda++;
		}
		// Luces apagadas
		else {
			spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
			spotLights[4].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
			spotLights[5].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		// Piso general**************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1200.0f, 1.0f, 1200.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoGeneralT.UseTexture();
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		
		// Barda que delimita la feria***********************************************************************
		// Al frente entrada ++++++++++++++++++++++++++++++++++++++++++++++++++
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 13.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 27.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 54.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 54.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 67.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, 81.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -13.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -27.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -27.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -54.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -67.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -81.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		// Derecho al frente ++++++++++++++++++++++++++++++++++++++++++++++++++
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(44.25f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.75f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.25f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.75f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.75f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.25f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-36.75f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.25f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-63.75f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-77.25f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.75f, -2.0f, -88.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		// Izquierdo al frente ++++++++++++++++++++++++++++++++++++++++++++++++++
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(44.25f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.75f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.25f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.75f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.75f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.25f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-36.75f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.25f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-63.75f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-77.25f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.75f, -2.0f, 86.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		// Atras entrada ++++++++++++++++++++++++++++++++++++++++++++++++++
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 13.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 27.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 40.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 54.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 67.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, 81.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -13.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -27.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -40.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -54.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -67.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -81.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-98.75f, -2.0f, -83.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bardaM.RenderModel();
		
		// Torre de madera***********************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, -5.5f, -55.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		torreMaderaM.RenderModel();
		
		// Enfermeria****************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-84.0f, -2.2f, 65.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		enfermeriaM.RenderModel();
		glDisable(GL_BLEND);
		
		// Letrero*******************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		letreroM.RenderModel();

		// Luz
		if (mainWindow.getLuzLetrero() == 1) {
			if (contLuzLetrero >= 0.0f && contLuzLetrero < 100.0f) {
				pointLights[2].SetColor(glm::vec3(0.0f, 0.9f, 0.9f));
				/*pointLights[2].SetPos(glm::vec3(102.8f + mainWindow.getMovLuzPointX(),
												23.8f + mainWindow.getMovLuzPointY(),
												0.0f + mainWindow.getMovLuzPointZ()));*/
			}
			else if (contLuzLetrero >= 100.0f && contLuzLetrero < 200.0f) {
				pointLights[2].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
				/*pointLights[2].SetPos(glm::vec3(102.8f + mainWindow.getMovLuzPointX(),
												23.8f + mainWindow.getMovLuzPointY(),
												0.0f + mainWindow.getMovLuzPointZ()));*/
			}
			else if (contLuzLetrero >= 200.0f && contLuzLetrero < 300.0f) {
				pointLights[2].SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
				/*pointLights[2].SetPos(glm::vec3(102.8f + mainWindow.getMovLuzPointX(),
												23.8f + mainWindow.getMovLuzPointY(),
												0.0f + mainWindow.getMovLuzPointZ()));*/
			}
			else {
				contLuzLetrero = -1.0f;
			}
			contLuzLetrero++;
		}
		else {
			pointLights[2].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		
		// Banca*******************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.8f, 3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bancaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.8f, -8.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bancaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(48.0f, -1.8f, -27.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.15f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bancaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.8f, 14.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bancaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.8f, 25.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bancaM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.0f, -1.8f, 26.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.15f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bancaM.RenderModel();
		
		// Souvenirs*******************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, -2.0f, 70.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		souvenirsM.RenderModel();
		
		// Lampara*******************************************************************************************
		// Entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(47.0f, -3.0f, -48.3f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lamparaM.RenderModel();
	
		// Salida
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(47.0f, -3.0f, 48.3f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lamparaM.RenderModel();

		// Prender y encender luz lampara entrada y salida
		// Sincronización con el skybox
		if (prenderApagarLES == 0) {
			// Luz entrada
			pointLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
			// Luz salida
			pointLights[1].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		else {
			// Luz entrada
			pointLights[0].SetColor(glm::vec3(1.0f, 1.0f, 0.5f));
			// Luz salida
			pointLights[1].SetColor(glm::vec3(1.0f, 1.0f, 0.5f));
		}

		// Glogo aerostático ********************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, -2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		globoM.RenderModel();

		// Puesto hotdogs ************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 2.0f, -63.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		carroComidaM.RenderModel();

		// Puesto helados ************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 2.0f, -63.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		carroHeladoM.RenderModel();

		// Baños ************************************************************************************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -2.0f, -35.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		banoM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -2.0f, -28.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		banoM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -2.0f, 35.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		banoM.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -2.0f, 28.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		banoM.RenderModel();

		// Vegetación ***************************************************************************************
		// Al frente+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Cactus tipo 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(46.0f, -1.0f, -25.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cactus1T.UseTexture();
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glDisable(GL_BLEND);

		// Cactus tipo 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(42.0f, 0.0f, -28.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cactus4T.UseTexture();
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glDisable(GL_BLEND);

		// Cactus tipo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(46.0f, -1.0f, 25.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cactus2T.UseTexture();
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glDisable(GL_BLEND);

		// Cactus tipo 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(43.0f, 0.0f, 28.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cactus3T.UseTexture();
		materialOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glDisable(GL_BLEND);
		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}