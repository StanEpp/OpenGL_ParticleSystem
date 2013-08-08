#ifndef _PARTICLEMANAGER_
#define _PARTICLEMANAGER_

#include "BufferBaseModel.h"
#include "Particle.h"

class ParticleManager{
private:
	BufferBaseModel		_bufferData;

	void setParticles(struct Particle* particles, int numParticles, int iniRadius);
	void deleteParticleManager();

public:
	ParticleManager() : _bufferData() {}
	~ParticleManager(){
		deleteParticleManager();
	}

	void loadParticleBuffer(int numParticles, int iniRadius);

	void loadUintUniform(GLuint shaderProgramID, std::string name, GLuint value);
	void loadFloatUniform(GLuint shaderProgramID, std::string name, GLfloat value);
	void loadVec4Uniform(GLuint shaderProgramID, std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void loadMatrix4Uniform(GLuint shaderProgramID, std::string name, const GLfloat* value);

	GLuint getParticleBufferID();
};


#endif