

#include "Particle.h"


Particle::Particle(glm::vec3 position, float mass, float damping, glm::vec2 texCoord)
{
	this->position = position;
	this->mass = mass;
	this->velocity = glm::vec3(0,0,0);
	this->texCoord = texCoord;
	this->lastPos = position;
	this->damping = damping;

	total_force = glm::vec3(0,0,0);
	isMovable = true;
	
}

void Particle::move(glm::vec3 delta)
{
	//update the particle velocity and position by delta.
	if(isMovable) {
	position += delta;
	 //movement in particle.
}

}

Particle::~Particle() {
	// TODO Auto-generated destructor stub
}

//euler explicit integrator timeStep = 1, damping = 0.00005
void Particle::eulerStep(float timeStep)
{	timeStep = 1;
	damping = 0.0025;
	if (!isMovable) return;	
	//get the acceleration for this timestep //newtonian force
	
	glm::vec3 temp = position;
	
	//euler integration
	position = position + velocity * (timeStep);
	velocity = velocity * damping + (total_force/mass) *(timeStep);
	lastPos = temp;	
	//clear out the forces
	total_force = glm::vec3(0,0,0);	
}

//vertlet integrator timeStep = 1000/60 damping = 0.01
void Particle::Vertletstep(float timeStep) {
	
if (isMovable) {
	//apply force
	glm::vec3 temp = position;
	position += (position-lastPos) * damping + (total_force/mass)*(timeStep/100);
	lastPos = temp;
//	move(position-lastPos);
	total_force = glm::vec3(0,0,0);
}

}

//Symplectic Euler integrator
 void Particle::symplecticEuler(float timeStep)
 {
	 timeStep = 1;
	 if (!isMovable) return;
	 //get the acceleration for this timestep //newtonian force
	 velocity = velocity * damping; //derivative of position is velocity
	 glm::vec3 temp = position;

	 //euler integration
	 velocity = velocity + (total_force / mass) * (timeStep);
	 position = position + velocity * (timeStep);

	 lastPos = temp;
	 //clear out the forces
	 total_force = glm::vec3(0, 0, 0);	
 	
 }
 
 
 void Particle::RungeKutta(float timeStep)
 {



 }
