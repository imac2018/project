#define GLEW_STATIC
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <string>
#include <SDL/SDL.h>
#include <streambuf>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

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
	VertexColor=1,
	VertexNormal,
	VertexTexCoord
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



		// Tableau des attributs des sommets
	GLfloat vertices[] = {
		-0.5, -0.5, 1.,0.,0., // Premier vertex
		0.5, -0.5, 0.,1.,0., // Deuxième vertex
		0., 0.5, 0., 0., 1. // Troisème vertex
	};

	// Création d'un Vertex Buffer Object et d'un Vertex Array Object
	GLuint vbo, vao;

	// Allocation d'un Vertex Buffer Object:
	glGenBuffers(1, &vbo);

	// "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Stockage des données du tableau vertices dans le vbo placé sur GL_ARRAY_BUFFER (c'est à dire "vbo" ici) :
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	// Activation de l'attribut de vertex 1; nous l'interpretons comme la couleur
	glEnableVertexAttribArray(VertexColor);

	// On spécifie le type de donnée de l'attribut position ainsi que la manière dont il est stocké dans le VBO
	glVertexAttribPointer(
		VertexPosition /* Indice attribut */,
		2 /* Nombre de composantes */,
		GL_FLOAT /* Type d'une composante */,
		GL_FALSE /* Pas de normalisation */,
		5 * sizeof(GLfloat) /* Taille en octet d'un vertex complet entre chaque attribut position */,
		0 /* OpenGL doit utiliser le VBO attaché à GL_ARRAY_BUFFER et commencer à l'offset 0 */);

	// On spécifie le type de donnée de l'attribut couleur ainsi que la manière dont il est stocké dans le VBO
	glVertexAttribPointer(
		VertexColor /* Indice attribut */,
		3 /* Nombre de composantes */,
		GL_FLOAT /* Type d'une composante */,
		GL_FALSE /* Pas de normalisation */,
		5 * sizeof(GLfloat) /* Taille en octet d'un vertex complet entre chaque attribut position */,
		(const GLvoid*)(2* sizeof(GLfloat)) /* OpenGL doit utiliser le VBO attaché à GL_ARRAY_BUFFER et commencer à l'offset 2 */);

	// Plus de VAO courant:
	glBindVertexArray(0);

	bool _continue = true;
	while(_continue){
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				_continue = false; // Leave the loop after this iteration
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);

		SDL_Delay(1000/60);
		SDL_GL_SwapBuffers();
	}
	SDL_Quit();
	return 0;
}
