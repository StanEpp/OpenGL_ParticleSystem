#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\gl3w.h>
	#include <GLFW\glfw3.h>
#endif

#define WORK_GROUP_SIZE 256
#define GLM_FORCE_RADIANS

#include <iostream>
#include <chrono>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "ShaderManager.h"
#include "ParticleBuffer.h"
#include "ParticleTexture.h"
#include "Attractor.h"
#include "GLFWWindow.h"
#include "GLFWInput.h"
#include "Timer.h"
#include "FirstPersonCamera.h"
#include "Camera.h"
#include "ConfigLoader.h"

struct Vertex{
  glm::vec4 pos;
};

class ParticleSystem{
private:
  GLFWWindow  _window;
  GLFWInput   _input;
  Camera<FirstPersonCamera> _camera;
  Attractor   _attractor;
  ParticleBuffer  _particleBuffer;
  ParticleTexture _particleTexture;
  GLuint  _vertexUVBufferID;
  GLuint  _vertexArrayID;
  float   _quadLength;
  bool    _showFPS;
  GLuint  _computeProgID, _shaderProgID;

  ShaderManager  _shaderManager;

  void render(double, double);
  void deleteParticleSystem() noexcept;
  
public:
  ParticleSystem() = delete;
  ParticleSystem(const Config&);
  ParticleSystem(ParticleSystem&) = delete;
  ParticleSystem(ParticleSystem&&) = delete;
  ParticleSystem& operator=(ParticleSystem&) = delete;
  ParticleSystem& operator=(ParticleSystem&&) = delete;
  ~ParticleSystem();

  void initialize();
  void run();
  void resize(int width, int height);
};


#endif