#ifndef _CAMERA_
#define _CAMERA_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLFWInput.hpp"

template<class UpdatePolicy>
class Camera{
private:
  glm::mat4 _projectionMatrix;
  float _fov, _nearPlane, _farPlane;
  int _width, _height;
  
  glm::mat4  _viewMatrix, _rotMatrix;
  glm::vec4  _lookTo, _up, _right, _position;
  float _xRot, _yRot, _zRot;
  float _speed, _sensitivity;
  
  friend UpdatePolicy;
  UpdatePolicy _updatePolicy;
  
  void setProjectionMatrix(){
    auto aspectRatio = static_cast<float>(_width)/static_cast<float>(_height);
    _projectionMatrix = glm::perspective(_fov, aspectRatio, _nearPlane, _farPlane);
  }
  
public:
  Camera() = delete;
  Camera(float speed, float sensitivity, float fov, int screenWidth, int screenHeight, float nearPlane, float farPlane) : 
    _projectionMatrix(), _fov(fov), _nearPlane(nearPlane), _farPlane(farPlane),
    _width(screenWidth), _height(screenHeight),
    _viewMatrix(), _rotMatrix(),
    _lookTo(0, 0, 0, 1), _up(), _right(), _position(0, 0, 0, 1),
    _xRot(0), _yRot(0), _zRot(0),
    _speed(speed), _sensitivity(sensitivity)
    {
      setProjectionMatrix();
    }

  void updateCamera(double dt, const GLFWInput& input){
    _updatePolicy.updateCamera(*this, input, dt);
  }
  
  void resize(int width, int height){
    _width = width; 
    _height = height;
    setProjectionMatrix();
  }
  
  glm::mat4 getProjectionMatrix() const{
    return _projectionMatrix;
  }
  
  float getFOV() const{
    return _fov;
  }
  
  float getNearPlane() const{
    return _nearPlane;
  }
  
  float getFarPlane() const{
    return _farPlane;
  }
  
  int getWidth() const{
    return _width;
  }
  
  int getHeight() const{
    return _height;
  }

  glm::mat4 getViewMatrix() const{
    return _viewMatrix;
  }

  glm::vec4 getRotMatrix() const{
    return  _rotMatrix;
  }

  glm::vec4 getPosition() const{
    return _position;
  }
  
  glm::vec4 getLookToVector() const{
    return _lookTo;
  }

  glm::vec4 getUpVector() const{
    return _up;
  }

  glm::vec4 getRightVector() const{
    return _right;
  }

  glm::vec4 getRotationVector() const{
    return glm::vec4(_xRot, _yRot, _zRot, 0.f);
  }
  
  void  setPosition(const glm::vec4& position){
    _position = position;
  }

};


#endif
