
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/fast_square_root.hpp"
#include "Shader.h"
#include "Cloth.h"
#include "Sphere.h"


GLuint mainWnd;
GLuint winWidth = 640;
GLuint winHeight = 480;
GLuint timeStep = 1000/60;

Shader* shader;
Cloth* cloth;
#define numThings 1
Sphere* spheres[numThings];

void timer(int t) {
	cloth->step(timeStep);
	for (int i = 0; i < numThings; i++)
		spheres[i]->collision(&(cloth->particles));
	glutPostRedisplay();  //calls display()
	glutTimerFunc(timeStep, timer, t);
}

void reshape(int w, int h) {
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
}

void setCamera() {
	//projection matrix
	glm::mat4 projection = glm::perspective((float)(20 * M_PI / 180), (float)winWidth / (float)winHeight, 1.f, 1000.f);
	shader->setProjectionMatrix(projection);

	//view matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(-100, 50, 300), //eye
		glm::vec3(0, -20, 0), //center
		glm::vec3(0, 1, 0) //up
	);
	shader->setViewMatrix(view);
}

void setLight() {
	float direction[] = { 0.5f, 1.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
}

void display() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("%s\n", gluErrorString(err));
	}

	setCamera();
	setLight();
	shader->setModelMatrix(glm::mat4());
	glColor3f(1, 1, 1);
	if (cloth != NULL)
	//cloth->display(GL_TRIANGLES);
	//cloth->display(GL_LINE_LOOP);
	cloth->displayConstraints();
	for (int i = 0; i < numThings; i++)
		spheres[i]->display(shader);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

void keyboardHandler(unsigned char key, int x, int y) {
	if (key == 'w') spheres[0]->move(glm::vec3(0, 0, -1));
	if (key == 's') spheres[0]->move(glm::vec3(0, 0, 1));
	if (key == 'a') spheres[0]->move(glm::vec3(-1, 0, 0));
	if (key == 'd') spheres[0]->move(glm::vec3(1, 0, 0));
	if (key == 'r') spheres[0]->move(glm::vec3(0, -1, 0));
	if (key == 'f') spheres[0]->move(glm::vec3(0, 1, 0));
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);

	mainWnd = glutCreateWindow("Cloth");

	glewInit();
	shader = new Shader((char*)"phong.vert", (char*)"phong.frag");
	shader->use();

	glm::mat4 transform = glm::translate(glm::vec3(-50, 20, 25)) * glm::rotate(30.f, glm::vec3(1, 0, 0)); //transform on the cloth

	cloth = new Cloth(100, 100, 100, 50, 150, 0.01, transform);
	cloth->loadTexture((char*)"redCloth.png");
	spheres[0] = new Sphere(glm::vec3(0, -30, 60), 20, glm::vec4(0.2, 0.8, 0.2, 1));
	//things[1] = new Floor(glm::vec3(0,-50,0), 400, 400, glm::vec4(1, 0, 0, 1));

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardHandler);

	timer(0);

	glutMainLoop();

	return EXIT_SUCCESS;
}
