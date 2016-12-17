#define GLEW_STATIC
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <string>
#include <memory>
#include <SDL/SDL.h>
#include <streambuf>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/FreeflyCamera.hpp>

#ifdef WIN32
void initGLextensions() {
//	glGetString= (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
}
#include <windows.h>
#else
#include <unistd.h>
#endif


enum GLATTRIBUT {
	VertexPosition=0,
	VertexNormal,
	VertexTexCoord,
	VertexColor
};

#define ERROR_EXIT(message) std::cerr << message << std::endl; exit(1);

using namespace std;
using namespace glm;
using namespace glimac;

int main(int argc, char *argv[])
{
	if(0 != SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}
	if(!SDL_SetVideoMode(500, 500, 32, SDL_OPENGL)) {
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_WM_SetCaption("Yolo", NULL);
	// Initialisation de GLEW; nous verrons dans le prochain TP à quoi cela sert.
	GLint error;
	if(GLEW_OK != (error = glewInit())) {
		std::cerr << "Impossible d'initialiser Glew" << std::endl;
		return 1;
	}
	glEnable(GL_DEPTH_TEST);

	Program program = loadProgram("vertex.vs.glsl", "fragment.fs.glsl");
	program.use();

	GLuint programID = program.getGLId();
	GLint uTexture, uMVPMatrix, uMVMatrix, uNormalMatrix;
	uTexture = glGetUniformLocation(programID, "uTexture");
	uMVPMatrix = glGetUniformLocation(programID, "uMVPMatrix");
	uMVMatrix = glGetUniformLocation(programID, "uMVMatrix");
	uNormalMatrix = glGetUniformLocation(programID, "uNormalMatrix");

	glm::mat4 ProjMatrix;
	ProjMatrix = glm::perspective<float>(glm::radians(70.f), 600.f/600.f, 0.1f, 100.f);

		// Tableau des attributs des sommets
	Sphere earth(4,20,20);

	// Création d'un Vertex Buffer Object et d'un Vertex Array Object
	GLuint vbo, vao;

	// Allocation d'un Vertex Buffer Object:
	glGenBuffers(1, &vbo);

	// "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Stockage des données du tableau vertices dans le vbo placé sur GL_ARRAY_BUFFER (c'est à dire "vbo" ici) :
	glBufferData(GL_ARRAY_BUFFER, earth.getVertexCount() * sizeof(ShapeVertex), earth.getDataPointer(), GL_STATIC_DRAW);

	// Plus de VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Allocation d'un Vertex Array Objet:
	glGenVertexArrays(1, &vao);

	// "vao" devient le VAO courant:
	glBindVertexArray(vao);

		// "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Activation de l'attribut de vertex 0; nous l'interpretons comme la position
	glEnableVertexAttribArray(VertexPosition);
	glEnableVertexAttribArray(VertexNormal);
	glEnableVertexAttribArray(VertexTexCoord);

	glVertexAttribPointer(
		VertexPosition /* Indice attribut */,
		3 /* Nombre de composantes */,
		GL_FLOAT /* Type d'une composante */,
		GL_FALSE /* Pas de normalisation */,
		sizeof(ShapeVertex) /* Taille en octet d'un vertex complet entre chaque attribut position */,
		(GLvoid*) offsetof(ShapeVertex, position));

	glVertexAttribPointer(VertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (GLvoid*) offsetof(ShapeVertex, normal));
	glVertexAttribPointer(VertexTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (GLvoid*) offsetof(ShapeVertex, texCoords));

	// Plus de VAO courant:
	glBindVertexArray(0);
	std::unique_ptr<Image> earthTexture =
			loadImage("asset/texture.jpg");

	GLuint earthTexId;
	glGenTextures(1, &earthTexId);
	glBindTexture(GL_TEXTURE_2D, earthTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthTexture->getWidth(), earthTexture->getHeight(),
				 0, GL_RGBA, GL_FLOAT, earthTexture->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeflyCamera camera;
	float vitesseF = 0, vitesseL = 0;

	bool _continue = true;
	while(_continue){
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				_continue = false; // Leave the loop after this iteration
			}

			if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
				case SDLK_w:
					vitesseF = 0.2;
					break;
				case SDLK_a:
					vitesseL = 0.2;
					break;
				case SDLK_s:
					vitesseF = -0.2;
					break;
				case SDLK_d:
					vitesseL = -0.2;
					break;
				default:
					break;
				}

			}
			if (e.type == SDL_KEYUP){
				Uint8* keyState = SDL_GetKeyState(nullptr);
				switch(e.key.keysym.sym){
				case SDLK_w:
					if(!keyState[SDLK_s])
						vitesseF = 0;
					break;
				case SDLK_s:
					if(!keyState[SDLK_w])
						vitesseF = 0;
					break;
				case SDLK_a:
					if(!keyState[SDLK_d])
						vitesseL = 0;
					break;
				case SDLK_d:
					if(!keyState[SDLK_a])
						vitesseL = 0;
					break;
				case SDLK_ESCAPE:
					camera = FreeflyCamera();
					break;
				default:
					break;
				}
			}
			if (e.type == SDL_MOUSEMOTION){
				if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
					camera.rotateLeft(e.motion.xrel*0.1);
					camera.rotateUp(e.motion.yrel*0.1);
				}
			}
		}

		camera.moveFront(vitesseF);
		camera.moveLeft(vitesseL);

		glm::mat4 camMatrix = camera.getViewMatrix();

		glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(camMatrix));
		glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(camMatrix))));
		glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * camMatrix));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthTexId);
		glUniform1i(uTexture,0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,earth.getVertexCount());
		glBindVertexArray(0);

		SDL_Delay(1000/60);
		SDL_GL_SwapBuffers();
	}
	SDL_Quit();
	return 0;
}
