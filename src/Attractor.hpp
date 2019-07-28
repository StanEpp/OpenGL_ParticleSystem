#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "Camera.hpp"
#include "GLFWInput.hpp"

class Attractor{
private:
  glm::vec4 m_position;
  float     m_attDepth; //The z-Coordinate of the attractor
  bool      m_active;

public:
  Attractor() : m_position(), m_attDepth(0), m_active(false) {}
  Attractor(glm::vec4& position) : m_position(position), m_attDepth(0), m_active(false) {}

  glm::vec4 pos() const{ return m_position; }
  float depth() const { return m_attDepth; }
  bool active() const { return m_active; }

  void updateAttractor(const Camera& camera, const GLFWInput& input){
    // When right mouse button is not pressed the attractor is not used.
    if(!input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
      m_active = false;
      return;
    }

    // Update attractor depth
    if(input.isKeyPressed(GLFW_KEY_E)){
      incrementDepth(1.0f);
    }
    if(input.isKeyPressed(GLFW_KEY_Q)){
      incrementDepth(-1.0f);
    }

    if(input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
      m_active = true;
    } else {
      m_active = false;
    }

    glm::vec3 view, rightVec, upVec, pos, dir, attractor(0);
    auto width = static_cast<float>(camera.width());
    auto height = static_cast<float>(camera.height());
    float aspectRatio = width/height;

    // Get coordinate axes
    view = glm::vec3(camera.lookDir());
    rightVec = glm::vec3(camera.right());
    upVec = glm::vec3(camera.up());

    // Compute half height of near plane in camera coordinate system
    float vLength = std::tan((camera.fovY()/2.f) * 180.f / std::acos(-1)) * camera.nearPlane();

    upVec *= vLength;
    rightVec *= vLength * aspectRatio;

    // Get mouse coordinates in screen coordinate system and map to [-1, 1]
    float mouseX = std::clamp(static_cast<float>(input.getXPos()), 0.f, width);
    float mouseY = std::clamp(static_cast<float>(input.getYPos()), 0.f, height);
    mouseX = (mouseX - (width/2.0f)) / (width/2.0f);
    mouseY = (mouseY - (height/2.0f)) / (height/2.0f);

    // Compute the intersection with the near plane
    pos = glm::vec3(camera.pos()) + view * camera.nearPlane() + rightVec * mouseX - upVec * mouseY;
    // Compute the direction of the ray
    dir = glm::normalize(pos - glm::vec3(camera.pos()));

    // Shoot attractor along the ray to the given depth
    attractor = pos + dir * m_attDepth;

    // Update attractor
    m_position = glm::vec4(attractor, 1.0f);
  }

  void incrementDepth(float inc)
  {
    m_attDepth += inc;
    if (m_attDepth < 0) {
      m_attDepth = 0;
    } else if (m_attDepth > 1000) {
      m_attDepth = 1000;
    }
  }
};