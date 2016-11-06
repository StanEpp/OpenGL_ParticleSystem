#ifndef _FIRSTPERSONCAMERA_
#define _FIRSTPERSONCAMERA_

#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "Camera.hpp"
#include "GLFWInput.hpp"

class FirstPersonCamera {
public:

  void updateCamera(Camera<FirstPersonCamera>& camera, const GLFWInput& input, double dt){
    float dz, dx, dy, dyRot, dxRot, dzRot;
    dz = dx = dy = dyRot = dxRot = dzRot = 0.f;
    
    if (input.isKeyPressed(GLFW_KEY_W)) {
      dz = static_cast<float>(static_cast<double>(camera._speed) * dt);
    }

    if (input.isKeyPressed(GLFW_KEY_S)) {
      dz = static_cast<float>(static_cast<double>(-1.f * camera._speed) * dt);
    }

    if (input.isKeyPressed(GLFW_KEY_D)) {
      dx = static_cast<float>(static_cast<double>(camera._speed) * dt);
    }

    if (input.isKeyPressed(GLFW_KEY_A)) {
      dx = static_cast<float>(static_cast<double>(-1.f * camera._speed) * dt);
    }
    
    if(!input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
      dyRot = -static_cast<float>(input.getXPosDiff() * static_cast<double>(camera._sensitivity) * dt);
      dxRot = -static_cast<float>(input.getYPosDiff() * static_cast<double>(camera._sensitivity) * dt);
    }
    
    glm::vec4 up, lookTo, right, position;
    glm::mat4 rotationMatrix, rotX, rotY, rotZ;

    up        = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    lookTo    = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
    position  = glm::vec4(camera.getPosition());

    camera._xRot += dxRot;
    camera._yRot += dyRot;
    camera._zRot += dzRot;

    rotX = glm::rotate(glm::mat4(1.f), glm::radians(camera._xRot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotY = glm::rotate(glm::mat4(1.f), glm::radians(camera._yRot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotZ = glm::rotate(glm::mat4(1.f), glm::radians(camera._zRot), glm::vec3(0.0f, 0.0f, -1.0f));

    rotationMatrix = rotZ * rotY * rotX;

    lookTo  = glm::normalize(rotationMatrix * lookTo);
    up  = glm::normalize(rotationMatrix * up);
    right = glm::normalize(glm::vec4(glm::cross(glm::vec3(lookTo), glm::vec3(up)), 1.0f));

    position += lookTo * dz;
    position += right * dx;
    position += up * dy;

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(position), glm::vec3(lookTo + position), glm::vec3(up));

    camera._position = position;
    camera._lookTo = lookTo;
    camera._up = up;
    camera._right = right;
    camera._viewMatrix = viewMatrix;
    camera._rotMatrix = rotationMatrix;
  }
};


#endif