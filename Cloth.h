#ifndef CLOTH_H_
#define CLOTH_H_
#include "Particle.h"
#include "Constraint.h"
#include "Triangle.h"

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


class Cloth {
	
	public:
	Cloth(float width, float height, int particleWidth, int particleHeight, float weight, float damping, glm::mat4 transform);
	virtual ~Cloth();

	void loadTexture(char *filename);
	void display(GLenum mode);
	void displayConstraints();
	void step(float timeStep);
    std::vector<Particle> particles;
	

private:
	std::vector<Constraint> constraints;
	std::vector<Triangle> triangles;
	
	int particleWidth;
	int particleHeight;

	bool isTextured;
	GLuint textureID;	
};

#endif /* CLOTH_H_ */
