#ifndef _PARTICLEMANAGER_
#define _PARTICLEMANAGER_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <random>
#include <stdexcept>
#include <iostream>
#include "BufferBaseModel.h"
#include "Particle.h"

class ParticleManager{
private:
	BufferBaseModel		_bufferData;
	
	void setParticles(Particle* particles, int numParticles, int iniRadius);
	void deleteParticleManager() noexcept;

public:
	~ParticleManager(){
		deleteParticleManager();
	}

	void loadParticleBuffer(int numParticles, int iniRadius);

	void loadUintUniform(GLuint shaderProgramID, const std::string& name, GLuint value);
	void loadFloatUniform(GLuint shaderProgramID, const std::string& name, GLfloat value);
	void loadVec4Uniform(GLuint shaderProgramID, const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void loadMatrix4Uniform(GLuint shaderProgramID, const std::string& name, const GLfloat* value);

	GLuint getParticleBufferID() const;
};


#endif