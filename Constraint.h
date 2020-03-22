
#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

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

class Constraint {
public:
	Constraint(Particle *p1, Particle *p2);
	virtual ~Constraint();

	void satisfy();
	void display();

private:
	Particle *p1, *p2;
	float distance;
};

#endif /* CONSTRAINT_H_ */

