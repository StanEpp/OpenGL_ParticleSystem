#ifndef _PARTICLEBUFFER_
#define _PARTICLEBUFFER_

#ifndef _GL3W_
#define _GL3W_
  #include <GL/gl3w.h>
  #include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <random>
#include <glm/glm.hpp>

struct Particle{
  glm::vec4 currPosition;
  glm::vec4 prevPosition;
};

class ParticleBuffer{
public:
  ParticleBuffer();
  ParticleBuffer(unsigned int, int);
  ParticleBuffer(ParticleBuffer&) = delete;
  ParticleBuffer(ParticleBuffer&&) = delete;
  ParticleBuffer& operator=(ParticleBuffer&) = delete;
  ParticleBuffer& operator=(ParticleBuffer&&) = delete;
  ~ParticleBuffer();

  void initializeParticles();

  GLuint getParticleBufferID() const;
  unsigned int getNumParticles() const;
  
private:
  GLuint       _buffID;
  unsigned int _numParticles;
  int          _initRadius;
  
  
  void distributeParticles(Particle*);
  void deleteParticleBuffer() noexcept;
};


#endif
