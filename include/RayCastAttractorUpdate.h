#ifndef _GLMATTRACTORUPDATE_
#define _GLMATTRACTORUPDATE_

#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "Classes.h"
#include "Camera.h"

#define PI 3.14159265358979323846f

class RayCastAttractorUpdate : public IAttractorUpdate<glm::mat4, glm::vec4, float>{
private:
	float _fovYRad; 
	float _nearClipDistance;
	float _width, _height;
	float _hLength, _vLength;

public:
	RayCastAttractorUpdate(){}

	~RayCastAttractorUpdate(){}

	void initAttractor(int width, int height, float fovYDeg, float nearClipDistance){
		_width	= static_cast<float>(width);
		_height = static_cast<float>(height);
		_nearClipDistance = nearClipDistance;
		_fovYRad = fovYDeg * PI/180.f;
		_vLength = glm::tan(_fovYRad/2)*nearClipDistance;
		_hLength = _vLength * ((float)(width)/(float)(height));
	}

	/** Performs ray casting to update the attractor 
	**/
	virtual void updateAttractor(AttractorModel<glm::vec4, float>* attModel, Camera<glm::mat4, glm::vec4, float>* camera, GLFWInput* input){
		glm::vec3 view, h, v, pos, dir, attractor;

		attractor = glm::vec3(0);

		//Compute the coordinate axes
		view = glm::vec3(camera->getLookToVector());
		h = glm::vec3(camera->getRightVector());
		v = glm::vec3(camera->getUpVector());
		
		//Scale them
		h *= _hLength;
		v *= _vLength;
		
		float mouseX = ((float)(input->getXPos())-(_width/2.0f))/(_width/2.0f);	//Map the coordinate to [-1, 1]
		float mouseY = ((float)(input->getYPos())-(_height/2.0f))/(_height/2.0f); //Map the coordinate to [-1, 1]

		//Compute the intersection with the near plane
		pos = glm::vec3(camera->getPosition()) + view*_nearClipDistance + h*mouseX - v*mouseY;
		//Compute the direction of the ray
		dir = glm::normalize(pos - glm::vec3(camera->getPosition()));
		
		//Shoot attractor along the ray to the given depth
		attractor = pos + dir * (float)attModel->getDepth();
	
		//Update attractor
		attModel->setPosition(glm::vec4(attractor, 1.0f));

	}
};

#endif