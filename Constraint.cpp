

#include "Constraint.h"

Constraint::Constraint(Particle *p1, Particle *p2) {
	this->p1 = p1;
	this->p2 = p2;
	distance = glm::length(p1->position-p2->position);
}

Constraint::~Constraint() {
	// TODO Auto-generated destructor stub
}

void Constraint::satisfy() {
	float tearDistance = 60.0f;
	glm::vec3 v= p1->position - p2->position;
	v *= ((1.0f - distance/glm::length(v)) * 0.8f); //spring force
	p1->move(-v);
	p2->move(v);
}

void Constraint::display() {
	glBegin(GL_LINES);
	glVertex3fv(glm::value_ptr(p1->position));
	glVertex3fv(glm::value_ptr(p2->position));
	glEnd();
}
