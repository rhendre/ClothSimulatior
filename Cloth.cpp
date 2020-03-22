#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Cloth.h"

 
Cloth::Cloth(float width, float height, int particleWidth, int particleHeight, float weight, float damping, glm::mat4 transform) {
	this->particleWidth = particleWidth;
	this->particleHeight = particleHeight;
	this->isTextured = true;
	this->textureID = 0;

	float
		particleSeperationX =  width/particleWidth,
		particleSeperationY = height/particleHeight,
		numParticles = particleWidth * particleHeight,
		particleMass = numParticles/weight,
		particleDamping = 1-damping;
		
		//add particles	
		for (int y = 0; y < particleHeight; y++)
		for (int x = 0; x < particleWidth; x++)
		{
			particles.push_back(Particle(
					glm::vec3(transform * glm::vec4(x*particleSeperationX, -y*particleSeperationY, 0, 1)),
					particleMass, particleDamping,
					glm::vec2((float)x/(particleWidth-1), -(float)y/(particleHeight-1))
			));

		}
		//Create the triangles
	for (int y = 1; y < particleHeight; y++)
		for (int x = 1; x < particleWidth; x++) {
			Particle
				*p1 = &particles[(y-1)*particleWidth+x-1],
				*p2 = &particles[(y-1)*particleWidth+x],
				*p3 = &particles[y*particleWidth+x-1],
				*p4 = &particles[y*particleWidth+x];

			triangles.push_back(Triangle(p3, p1, p2));
			triangles.push_back(Triangle(p2, p4, p3));
		}

	//create constraints structural, shear and bending frames.
		 
	for (int y = 0; y < particleHeight; y++)
		for (int x = 0; x < particleWidth; x++) {

			//structural constraints
			if (x < particleWidth - 1) 
				constraints.push_back(Constraint(&particles[(y)*particleWidth + x], &particles[y * particleWidth + x + 1]));
			if(y < particleHeight -1)
				constraints.push_back(Constraint(&particles[(y)*particleWidth + x], &particles[(y + 1) * particleWidth + x]));
			
			if (x < particleWidth - 1 && y < particleHeight - 1) {
				//shear constraints

				constraints.push_back(Constraint(&particles[(y)*particleWidth + x], &particles[(y + 1) * particleWidth + (x + 1)]));
				constraints.push_back(Constraint(&particles[(y)*particleWidth + (x + 1)], &particles[(y + 1) * particleWidth + x]));
			}

			if (x < particleWidth - 2) constraints.push_back(Constraint (&particles[(y)*particleWidth + x], &particles[(y)*particleWidth + x+2]));
			if (y < particleHeight - 2) constraints.push_back(Constraint (&particles[(y)*particleWidth + x], &particles[(y+2)*particleWidth + x]));
			if (x < particleWidth - 2 && y < particleHeight - 2) constraints.push_back(Constraint (&particles[(y)*particleWidth + x], &particles[(y+2)*particleWidth + x+2]));
			if (x < particleWidth - 2 && y < particleHeight - 2) constraints.push_back(Constraint (&particles[(y)*particleWidth + x+2], &particles[(y+2)*particleWidth + x]));

		}
	
	//give the cloth a tiny push in the z direction
	particles[numParticles/2+particleWidth/2].total_force.z = 10.0;

	//Make some points static
	//particles[particleWidth/2].isMovable = false; //top middle
	//particles[0].isMovable = false; //top left
    //particles[particleWidth-1].isMovable = false; //top right
	
}

void Cloth::step(float timeStep) {
	//Gravity
	for (std::vector<Particle>::size_type i = 1; i < particles.size(); i++)
		particles[i].total_force += glm::vec3(0, -9.8, 0);

	//Satisfy constraints
	for (int n = 0; n < 50; n++)
		for (std::vector<Constraint>::size_type i = 0; i < constraints.size(); i++)
			constraints[i].satisfy();

	//euler step
	for (std::vector<Particle>::size_type i = 0; i < particles.size(); i++)    //please use the below functions to see the effect of different integrators on the cloth simulation
		 particles[i].eulerStep(timeStep);
	    //particles[i].Vertletstep(timeStep);
		//particles[i].symplecticEuler(timeStep);

}

Cloth::~Cloth() {
	// TODO Auto-generated destructor stub
}

void Cloth::display(GLenum mode)
{
		
		//Bind the texture
	if (isTextured) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

//Calculate normals
	for (int y = 0; y < particleHeight; y++)
		for (int x = 0; x < particleWidth; x++) {
			glm::vec3 vy, vx;
			Particle *p = &particles[y*particleWidth+x];

			if (y > 0) vy = particles[(y-1)*particleWidth+x].position;
			else vy = p->position;
			if (y < particleHeight) vy -= particles[(particleHeight-1)*particleWidth+x].position;
			else vy -= p->position;

			if (x > 0) vx = particles[y*particleWidth+x-1].position;
			else vx = p->position;
			if (x < particleWidth-1) vx -= particles[y*particleWidth+x+1].position;
			else vx -= p->position;

			p->normal = glm::normalize(glm::cross(vy, vx));
		}

	//draw triangles
	glBegin(mode);
	for (std::vector<Triangle>::size_type i = 0; i < triangles.size(); i++)
		triangles[i].display();
		glEnd();
		
			//Unbind the texture
	if (isTextured) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void Cloth::displayConstraints() {
	for (std::vector<Constraint>::size_type i = 0; i < constraints.size(); i++)
		constraints[i].display();
}



void Cloth::loadTexture(char* filename) {
	GLenum
		texture = GL_TEXTURE0,
		target = GL_TEXTURE_2D;

	//Init the texture storage, and set some parameters.
	glActiveTexture(texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Load the png file from disk and decode it
	//  (assumes the file is a png with OpenGL-friendly dimensions)
	int imageWidth, imageHeight, comp;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* imageData = stbi_load(filename, &imageWidth, &imageHeight, &comp, STBI_rgb);

	//Load the texture into GL
	glTexImage2D(target, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB,
			GL_UNSIGNED_BYTE, imageData);

	isTextured = true;
}








