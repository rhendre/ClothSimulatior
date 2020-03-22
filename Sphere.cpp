

#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float radius, glm::vec4 color) {
	this->center = center;
	this->radius = radius;
	this->color = color;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

void Sphere::collision(std::vector<Particle> *particles) {
	for (std::vector<Particle>::size_type i = 0; i < particles->size(); i++) {
		glm::vec3 v = (*particles)[i].position - center; //see the distance from the particles to the center of the sphere
		float distance = glm::length(v);
		if (distance < radius) {//if particle inside the sphere then put the particle in a normalized direction on the sphere
			(*particles)[i].position += glm::normalize(v) * (radius - distance);
			//correct it's distance due to frictional parameter
			(*particles)[i].position += glm::normalize(v)*0.0025f;
		}
		
	}
}

void Sphere::display(Shader *shader) {
	shader->setModelMatrix(glm::translate(center));
	glColor3fv(glm::value_ptr(color));
	glutSolidSphere(radius, 30, 30);
}

void Sphere::move(glm::vec3 delta) {
	center += delta;
}
