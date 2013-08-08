#ifndef _FREELOOKCAMERA_
#define _FREELOOKCAMERA_

#include "ICameraRender.h"
#include "CameraModel.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>

class FreeLookCamera : public ICameraRender<glm::mat4, glm::vec4, float> {
public:
	FreeLookCamera(){}
	~FreeLookCamera(){}

	virtual void updateCamera(CameraModel<glm::mat4, glm::vec4, float>* camera){
		glm::vec4 up, lookTo, right, position;
		glm::mat4 rotationMatrix, rotX, rotY, rotZ;

		up			= glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		lookTo		= glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		position	= glm::vec4(camera->getPosition());

		camera->setPitch(camera->getPitch()+camera->getAddPitch()); 
		camera->setYaw(camera->getYaw()+camera->getAddYaw()); 
		camera->setRoll(camera->getRoll()+camera->getAddRoll()); 

		rotX = glm::rotate(camera->getPitch(), 1.0f, 0.0f, 0.0f);
		rotY = glm::rotate(camera->getYaw(), 0.0f, 1.0f, 0.0f);
		rotZ = glm::rotate(camera->getRoll(), 0.0f, 0.0f, 1.0f);

		rotationMatrix = rotZ*rotY*rotX;

		lookTo	= rotationMatrix*lookTo;
		lookTo = glm::normalize(lookTo);

		up		= rotationMatrix*up;
		up		= glm::normalize(up);

		right	= glm::vec4(glm::cross(glm::vec3(lookTo), glm::vec3(up)), 1.0f);
		right	= glm::normalize(right);

		position += lookTo*camera->getAddZPos(); 
		position += right*camera->getAddXPos();
		position += up*camera->getAddYPos();

		camera->setAddXPos(0); 
		camera->setAddYPos(0);
		camera->setAddZPos(0);

		camera->setAddPitch(0);
		camera->setAddRoll(0);
		camera->setAddYaw(0);

		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(position), glm::vec3(lookTo+position), glm::vec3(up));

		camera->setPosition(position);
		camera->setLookToVector(lookTo);
		camera->setUpVector(up);
		camera->setViewMatrix(viewMatrix);
		camera->setRotMatrix(rotationMatrix);

	}
};


#endif