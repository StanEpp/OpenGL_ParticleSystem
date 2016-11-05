#include "ParticleBuffer.h"

ParticleBuffer::ParticleBuffer() : _buffID(0), _numParticles(1000), _initRadius(15) {}
ParticleBuffer::ParticleBuffer(unsigned int numParticles, int initRadius) : _buffID(0), _numParticles(numParticles), _initRadius(initRadius) {}

void ParticleBuffer::initializeParticles(){
  Particle* particles = new Particle[_numParticles];
  distributeParticles(particles);
  
  glGetError();
  
  glGenBuffers(1, &_buffID);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffID);
  glBufferData(GL_SHADER_STORAGE_BUFFER, _numParticles * sizeof(Particle), particles ,GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _buffID);
  
  delete particles;
  
  if(glGetError() != GL_NO_ERROR){
    throw std::runtime_error("ERROR: Could not generate or bind shader storage buffer for particles!");
  }
}

void ParticleBuffer::distributeParticles(Particle* particles){
  float rndX, rndY, rndZ;
  auto fInitRadius = static_cast<float>(_initRadius);
  std::mt19937 eng;
  std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

  for(unsigned int i = 0; i < _numParticles; ++i){
    rndX = dist(eng);
    rndY = dist(eng);
    rndZ = dist(eng);
    particles[i].currPosition = glm::vec4(rndX, rndY, rndZ, 1.f);
    particles[i].prevPosition = glm::vec4(rndX, rndY, rndZ, 1.f);
  }
}

GLuint ParticleBuffer::getParticleBufferID() const{
  return _buffID;
}

unsigned int ParticleBuffer::getNumParticles() const{
  return _numParticles;
}

void ParticleBuffer::deleteParticleBuffer() noexcept{
  glGetError();
  
  glDeleteBuffers(1, &_buffID);
  
  if(glGetError() != GL_NO_ERROR){
    std::cerr << "Could not properly delete particle Buffer!" << std::endl;
  }
}