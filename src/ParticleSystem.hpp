#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#define WORK_GROUP_SIZE 256
#define GLM_FORCE_RADIANS

#include <iostream>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderManager.hpp"
#include "ParticleBuffer.hpp"
#include "ParticleTexture.hpp"
#include "Attractor.hpp"
#include "GLFWWindow.hpp"
#include "GLFWInput.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "ConfigLoader.hpp"

struct Vertex{
  glm::vec4 pos;
};

class ParticleSystem{
private:
  Settings m_settings;
  GLFWWindow  m_window;
  GLFWInput   m_inputControl;
  Camera _camera;
  Attractor   _attractor;
  ParticleBuffer  _particleBuffer;
  ParticleTexture _particleTexture;
  GLuint  _vertexUVBufferID;
  GLuint  _vertexArrayID;
  float   _quadLength;
  bool    _showFPS;
  GLuint  _computeProgID, _shaderProgID;

  ShaderManager  _shaderManager;


  struct ComputeShaderUniformLocations {
    GLuint frameTimeDiff;
    GLuint attPos;
    ComputeShaderUniformLocations() : frameTimeDiff(0), attPos(0) {}
  } _csLocations;

  struct ParticleShaderUniformLocations {
    GLuint viewMatrix;
    GLuint camPos;
    GLuint time;
    ParticleShaderUniformLocations() : viewMatrix(0), camPos(0), time(0) {}
  } _psLocations;

  void render(double, double);
  void deleteParticleSystem() noexcept;

public:
  ParticleSystem() = delete;
  ParticleSystem(const std::string&);
  ParticleSystem(ParticleSystem&) = delete;
  ParticleSystem(ParticleSystem&&) = delete;
  ParticleSystem& operator=(ParticleSystem&) = delete;
  ParticleSystem& operator=(ParticleSystem&&) = delete;
  ~ParticleSystem();

  void initialize();
  void run();
};