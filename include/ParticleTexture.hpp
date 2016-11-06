#ifndef _PARTICLETEXTURE_
#define _PARTICLETEXTURE_

#ifndef _GLEW_
#define _GLEW_
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
  void useTexture(const GLuint shaderProgramID);

};

#endif