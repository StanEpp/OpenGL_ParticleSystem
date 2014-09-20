#ifndef _PARTICLE_
#define _PARTICLE_

#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>

struct Particle{
	glm::vec4	_currPosition;
	glm::vec4	_prevPosition;
};

#endif