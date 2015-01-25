#include "ParticleManager.h"

void ParticleManager::loadParticleBuffer(int numParticles, int iniRadius){
	glGetError();
	
	int _numParticles = numParticles;
	Particle* _particles = new Particle[_numParticles];
	setParticles(_particles, _numParticles, iniRadius);
	
	GLuint bufferID;
	
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles*sizeof(Particle), _particles ,GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferID);
	
	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error("ERROR: Could not generate or bind shader storage buffer for particles!");
	}

	_bufferData.saveBuffer("particleBuffer", bufferID);

}

void ParticleManager::setParticles(Particle* particles, int numParticles, int iniRadius){
	float rndX, rndY, rndZ;
	std::mt19937 eng;
	std::uniform_real_distribution<float> dist(static_cast<float>(iniRadius)*(-1.0f), static_cast<float>(iniRadius));

	for(int i = 0; i < numParticles; ++i){
		rndX = dist(eng);
		rndY = dist(eng);
		rndZ = dist(eng);
		particles[i]._currPosition = glm::vec4(rndX, rndY, rndZ, 1.0f);
		particles[i]._prevPosition = glm::vec4(rndX, rndY, rndZ, 1.0f);
	}
}

GLuint ParticleManager::getParticleBufferID() const{
	return _bufferData.getBufferID("particleBuffer");
}

void ParticleManager::loadUintUniform(GLuint shaderProgramID, const std::string& name, GLuint value){
	glGetError();

	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform1ui(_uniID, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::loadFloatUniform(GLuint shaderProgramID, const std::string& name, GLfloat value){
	glGetError();

	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform1f(_uniID, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::loadVec4Uniform(GLuint shaderProgramID, const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	glGetError();
	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform4f(_uniID, x, y, z, w);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::loadMatrix4Uniform(GLuint shaderProgramID, const std::string& name, const GLfloat* value){
	glGetError();
	int _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniformMatrix4fv(_uniID, 1, GL_FALSE, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ParticleManager::deleteParticleManager() noexcept{
	try{
		GLuint ID = _bufferData.getBufferID("particleBuffer");
		glDeleteBuffers(1, &ID);
		
		if(glGetError() != GL_NO_ERROR){
			throw std::runtime_error("");
		}
		
		_bufferData.deleteBuffer("particleBuffer");
	} catch(...){
		std::cout << "ERROR: Could not delete particleBuffer!" <<std::endl;
	}
}