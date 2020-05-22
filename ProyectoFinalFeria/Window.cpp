#include "Window.h"

Window::Window(){
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++){
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight){
	width = windowWidth;
	height = windowHeight;
	movLuzPointX = 0.0f;
	movLuzPointY = 0.0f;
	movLuzPointZ = 0.0f;
	movLuzSpotX = 0.0f;
	movLuzSpotY = 0.0f;
	movLuzSpotZ = 0.0f;
	luzCarrusel = 1;
	luzRueda = 1;
	luzLetrero = 1;
	camara = 0.0f;
	cambioCamara = 0.0f;
	recCam = 0;
	animacionCarrusel = 0;
	animacionRueda = 0;
	animacionDestreza = 0;

	for (size_t i = 0; i < 1024; i++){
		keys[i] = 0;
	}
}

int Window::Initialise(){
	// Inicialización de GLFW
	if (!glfwInit()){
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	// Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Feria LUFER", NULL, NULL);

	if (!mainWindow){
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	// Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	// MANEJAR TECLADO y MOUSE
	createCallbacks();

	// Permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK){
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas
	glEnable(GL_DEPTH_TEST); 
							 
	// Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	// Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks(){
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange(){
	GLfloat theChange = xChange;
	xChange = 0.0f;

	return theChange;
}

GLfloat Window::getYChange(){
	GLfloat theChange = yChange;
	yChange = 0.0f;

	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode){
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Teclas para controlar las animaciones de los juegos mecánicos
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		if (theWindow->animacionRueda == 0) {
			theWindow->animacionRueda = 1;
		}
		else {
			theWindow->animacionRueda = 0;
		}
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		if (theWindow->animacionCarrusel == 0) {
			theWindow->animacionCarrusel = 1;
		}
		else {
			theWindow->animacionCarrusel = 0;
		}
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (theWindow->animacionDestreza == 0) {
			theWindow->animacionDestreza = 1;
		}
		else {
			theWindow->animacionDestreza = 0;
		}
	}

	// Teclas para controlar el cambio de camaras
	// Camara general
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		theWindow->cambioCamara = 0.0f;
		theWindow->camara = 1.0f;
	}

	// Camara carrusel
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		theWindow->cambioCamara = 0.0f;
		theWindow->camara = 2.0f;
	}

	// Camara rueda
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		theWindow->cambioCamara = 0.0f;
		theWindow->camara = 3.0f;
	}

	// Camara tiendas
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		theWindow->cambioCamara = 0.0f;
		theWindow->camara = 4.0f;
	}

	// Camara aerea
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		theWindow->cambioCamara = 0.0f;
		theWindow->camara = 5.0f;
	}

	// Camara recocorrido
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (theWindow->recCam == 1) {
			theWindow->recCam = 0;
		}
		else {
			theWindow->recCam = 1;
		}
	}

	// Movimiento de la luz pointlight
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		theWindow->movLuzPointX += 2.0;
		printf("x: %f\n", theWindow->movLuzPointX);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		theWindow->movLuzPointX -= 2.0;
		printf("X: %f\n", theWindow->movLuzPointX);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		theWindow->movLuzPointY += 2.0;
		printf("y: %f\n", theWindow->movLuzPointY);
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		theWindow->movLuzPointY -= 2.0;
		printf("y: %f\n", theWindow->movLuzPointY);
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		theWindow->movLuzPointZ += 2.0;
		printf("z: %f\n", theWindow->movLuzPointZ);
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		theWindow->movLuzPointZ -= 2.0;
		printf("z: %f\n", theWindow->movLuzPointZ);
	}

	// Movimiento de la luz spotligth
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		theWindow->movLuzSpotX += 2.0;
		printf("x: %f\n", theWindow->movLuzSpotX);
	}

	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		theWindow->movLuzSpotX -= 2.0;
		printf("X: %f\n", theWindow->movLuzSpotZ);
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		theWindow->movLuzSpotY += 2.0;
		printf("y: %f\n", theWindow->movLuzSpotY);
	}
	
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		theWindow->movLuzSpotY -= 2.0;
		printf("y: %f\n", theWindow->movLuzSpotY);
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		theWindow->movLuzSpotZ += 2.0;
		printf("z: %f\n", theWindow->movLuzSpotZ);
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		theWindow->movLuzSpotZ -= 2.0;
		printf("z: %f\n", theWindow->movLuzSpotZ);
	}

	// Luz carrusel tipo spotligth prender o apagar
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		if (theWindow->luzCarrusel == 1) {
			theWindow->luzCarrusel = 0;
		}
		else {
			theWindow->luzCarrusel = 1;
		}
	}
	
	// Luz rueda tipo spotligth prender o apagar
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		if (theWindow->luzRueda == 1) {
			theWindow->luzRueda = 0;
		}
		else {
			theWindow->luzRueda = 1;
		}
	}

	// Luz letrero tipo pointligth prender apagar
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (theWindow->luzLetrero == 1) {
			theWindow->luzLetrero = 0;
		}
		else {
			theWindow->luzLetrero = 1;
		}
	}

	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS){
			theWindow->keys[key] = true;
			printf("Se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE){
			theWindow->keys[key] = false;
			printf("Se solto la tecla %d'\n", key);
		}
	}
	
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos){
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved){
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

void Window::setCamara(GLfloat cam) {
	camara = cam;
}

Window::~Window(){
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
