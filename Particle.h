
#ifndef PARTICLE_H_
#define PARTICLE_H_

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


class Particle
{
	public:
		Particle(glm::vec3 position, float mass, float damping, glm::vec2 texCoord);
		virtual ~Particle();
		
		bool isMovable; //is not a static point
		glm::vec3 position; //(x,y,z) co-ordinates 
		glm::vec3 velocity; //derivative of position
		float mass; //static mass of each particle in cloth;
        glm::vec3 lastPos;
		glm::vec3 total_force;
		glm::vec3 normal;
        glm::vec2 texCoord;
        float damping;
		
	  void move(glm::vec3 delta);
	  void eulerStep(float timeStep);
      void Vertletstep(float timeStep);
	  void symplecticEuler(float timeStep);
	  void RungeKutta(float timeStep);
	
	
};

#endif /* PARTICLE_H_ */
