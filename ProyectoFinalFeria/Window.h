#pragma once

#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window{
	public:
		Window();
		Window(GLint windowWidth, GLint windowHeight);

		int Initialise();

		GLfloat getBufferWidth() { 
			return bufferWidth; 
		}
		GLfloat getBufferHeight() { 
			return bufferHeight; 
		}
		GLfloat getXChange();
		GLfloat getYChange();

		bool getShouldClose() {
			return  glfwWindowShouldClose(mainWindow);
		}
		bool* getsKeys() { 
			return keys; 
		}
		void swapBuffers() { 
			return glfwSwapBuffers(mainWindow); 
		}

		bool getAnimacionRueda() {
			return animacionRueda;
		}
		bool getAnimacionCarrusel() {
			return animacionCarrusel;
		}
		bool getAnimacionDestreza() {
			return animacionDestreza;
		}
		// Prende/Apagar luz tipo spotligth
		bool getLuzCarrusel() {
			return luzCarrusel;
		}
		// Prende/Apagar luz tipo spotligth
		bool getLuzRueda() {
			return luzRueda;
		}
		// Prende/Apagar luz tipo pointligth
		bool getLuzLetrero() {
			return luzLetrero;
		}
		// Activar/Desactivar recorrido camara
		bool getRecCam() {
			return recCam;
		}

		GLfloat getCamaraGeneral() {
			return camara;
		}
		GLfloat getMovLuzPointX() {
			return movLuzPointX;
		}
		GLfloat getMovLuzPointY() {
			return movLuzPointY;
		}
		GLfloat getMovLuzPointZ() {
			return movLuzPointZ;
		}

		GLfloat getMovLuzSpotX() {
			return movLuzSpotX;
		}
		GLfloat getMovLuzSpotY() {
			return movLuzSpotY;
		}
		GLfloat getMovLuzSpotZ() {
			return movLuzSpotZ;
		}

		void setCamara(GLfloat cam);

		GLfloat cambioCamara;

		~Window();

	private: 
		GLFWwindow *mainWindow;

		GLint width, height;
		GLint bufferWidth, bufferHeight;

		bool keys[1024];
		bool mouseFirstMoved;
		bool luzCarrusel;
		bool luzRueda;
		bool luzLetrero;
		bool recCam;
		GLfloat camara;

		void createCallbacks();

		GLfloat lastX;
		GLfloat lastY;
		GLfloat xChange;
		GLfloat yChange;

		GLfloat movLuzPointX, movLuzPointY, movLuzPointZ;
		GLfloat movLuzSpotX, movLuzSpotY, movLuzSpotZ;
		
		bool animacionRueda, animacionCarrusel, animacionDestreza;

		static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
		static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

