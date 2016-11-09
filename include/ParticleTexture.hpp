#ifndef _PARTICLETEXTURE_
#define _PARTICLETEXTURE_

#ifndef _GL3W_
#define _GL3W_
	#include <GL\gl3w.h>
	#include <GLFW\glfw3.h>
#endif

#include <stdexcept>
#include "tga.h"

class ParticleTexture{
private:
  GLuint _textureID;

  void deleteTexture() noexcept;

public:
  ParticleTexture();
  ParticleTexture(ParticleTexture&) = delete;
  ParticleTexture(ParticleTexture&&) = delete;
  ParticleTexture& operator=(ParticleTexture&) = delete;
  ParticleTexture& operator=(ParticleTexture&&) = delete;
  ~ParticleTexture();

  void loadTexture(const std::string& path);
  void useTexture(GLuint shaderProgramID);

};

#endif