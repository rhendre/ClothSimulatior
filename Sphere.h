

#ifndef SPHERE_H_
#define SPHERE_H_

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
#include "Particle.h"
#include "Shader.h"

class Sphere {
public:
	Sphere(glm::vec3 center, float radius, glm::vec4 color);
	virtual ~Sphere();

	void collision(std::vector<Particle> *particles);
	void display(Shader *shader);
	void move(glm::vec3 delta);

private:
	glm::vec4 color;
	float radius;
	glm::vec3 center;
};

#endif /* SPHERE_H_ */

