#include "ParticleManager.h"
#include <random>

void ParticleManager::loadParticleBuffer(int numParticles, int iniRadius){
	glGetError();

	GLuint bufferID;

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles*sizeof(Particle), NULL ,GL_STATIC_DRAW);

	if(glGetError() != GL_NO_ERROR){
		throw std::exception("ERROR: Could not generate or bind shader storage buffer for particles!");
	}

	_bufferData.saveBuffer("particleBuffer", bufferID);

	struct Particle* particles;

	try{
		particles = (struct Particle*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numParticles*sizeof(Particle), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	} catch(std::runtime_error e){
		std::cout << e.what() <<std::endl;
	}

	if(glGetError() != GL_NO_ERROR || particles == NULL){
		throw std::exception("ERROR: Could not map the shader storage buffer for particles!");
	}

	setParticles(particles, numParticles, iniRadius);

	if(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER) == GL_FALSE){
		throw std::exception("ERROR: Could not unmap the shader storage buffer for particles!");
	}

}

void ParticleManager::setParticles(struct Particle* particles, int numParticles, int iniRadius){
	float rndX, rndY, rndZ;
	std::tr1::mt19937 eng;
	std::tr1::uniform_real_distribution<float> dist(static_cast<float>(iniRadius)*(-1.0f), static_cast<float>(iniRadius));

	for(int i = 0; i < numParticles; ++i){
		rndX = dist(eng);
		rndY = dist(eng);
		rndZ = dist(eng);
		particles[i]._currPosition = glm::vec4(rndX, rndY, rndZ, 1.0f);
		particles[i]._prevPosition = glm::vec4(rndX, rndY, rndZ, 1.0f);
	}
}

GLuint ParticleManager::getParticleBufferID(){
	return _bufferData.getBufferID("particleBuffer");
}

void ParticleManager::loadUintUniform(GLuint shaderProgramID, std::string name, GLuint value){
	glGetError();

	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform1ui(_uniID, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::loadFloatUniform(GLuint shaderProgramID, std::string name, GLfloat value){
	glGetError();

	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform1f(_uniID, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::loadVec4Uniform(GLuint shaderProgramID, std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	glGetError();
	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform4f(_uniID, x, y, z, w);

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::loadMatrix4Uniform(GLuint shaderProgramID, std::string name, const GLfloat* value){
	glGetError();
	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniformMatrix4fv(_uniID, 1, GL_FALSE, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::deleteParticleManager(){
	GLuint ID = _bufferData.getBufferID("particleBuffer");
	glDeleteBuffers(1, &ID);
	if(glGetError() != GL_NO_ERROR){
		throw std::exception("ERROR: Could not delete particleBuffer!");
	}

	_bufferData.deleteBuffer("particleBuffer");
}