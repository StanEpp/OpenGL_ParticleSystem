#ifndef _GLMATTRACTORUPDATE_
#define _GLMATTRACTORUPDATE_

#include "Classes.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Camera.h"
#include "Input.h"

#define PI 3.14159265358979323846f

class GLMAttractorUpdate : public IAttractorUpdate<glm::mat4, glm::vec4, float>{
private:
	float _scaleY, _scaleX; 
	float _nearClipDistance;
	float _width, _height;

public:
	GLMAttractorUpdate(){}

	~GLMAttractorUpdate(){}

	void initAttractor(int width, int height, float fovYDeg, float nearClipDistance){
		_width	= static_cast<float>(width);
		_height = static_cast<float>(height);
		_nearClipDistance = nearClipDistance;

		//Is calculated with the vertical field of view of the projection matrix which is 45° in this particle system
		//and its distance from eye to the new clip space which 0.01.
		//The calculation goes like tan(45/2)*0.01 in degrees and tan(45/2*(180/PI))*0.01 in rad.
		//_scale = glm::tan((fovDeg/2.0f))*(180.0f/PI)*nearClipDistance;
		//The horizontal field of view gets calculated like this: fovX = 2*atan(tan(fovY/2)*(widht/height))*nearClipDistance.

		_scaleY = glm::tan(glm::radians(fovYDeg)/2.0f)*nearClipDistance;	//_scaleY = 0.004142135624f

		float fovXRad = 2*glm::atan(glm::tan(glm::radians(fovYDeg)/2)*static_cast<float>(width)/static_cast<float>(height));
		_scaleX = glm::tan(fovXRad/2.0f)*nearClipDistance;	//_scaleX = 0.005522847498

	}

	virtual void updateAttractor(AttractorModel<glm::vec4, float>* attModel, Camera<glm::mat4, glm::vec4, float>* camera, Input* input){
		glm::vec3 lookTo, up, right, pos, attractor;

		attractor = glm::vec3(0);

		lookTo = glm::normalize(glm::vec3(camera->getLookToVector()));
		up = glm::normalize(glm::vec3(camera->getUpVector()));
		right = glm::normalize(glm::vec3(glm::vec4(glm::cross(glm::vec3(lookTo), glm::vec3(up)), 1.0f)));
		pos = glm::vec3(camera->getPosition());
		
		float mouseX = ((float)input->getXPos()-(_width/2.0f))/(_width/2.0f);	//Map the coordinate to [-1, 1]
		float mouseY = ((float)input->getYPos()-(_height/2.0f))/(_height/2.0f); //Map the coordinate to [-1, 1]
		
		//Scale the coordinates so it fits to the fovY and fovX
		mouseY *= _scaleY;	
		mouseX *= _scaleX;  

		glm::vec4 forw = glm::vec4(lookTo, 1.0f);

		if(mouseY != 0.0f){
			float angleYDeg = glm::degrees(glm::atan(mouseY/_nearClipDistance));

			glm::mat4 rotX = glm::rotate(-angleYDeg, right);
			
			forw = rotX*forw;
		}

		if(mouseX != 0.0f){
			float angleXDeg = glm::degrees(glm::atan(mouseX/_nearClipDistance));

			glm::mat4 rotY = glm::rotate(-angleXDeg, up);

			forw = rotY*forw;
		}

		attractor = pos-right*0.2f-up*0.2f+glm::vec3(forw)*(float)input->getMouseWheel();

		attModel->setPosition(glm::vec4(attractor,1.0f));
	}
};

#endif