#ifndef _ATTRACTOR_
#define _ATTRACTOR_

#define GLM_FORCE_RADIANS

#include <memory>

#include <glm\glm.hpp>

#include "Classes.h"
#include "AttractorModel.h"
#include "IAttractorUpdate.h"
#include "Camera.h"
#include "GLFWInput.h"

template <class Matrix, class Vector, class Value>
class Attractor{
private:
	std::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> >	_updateStrategy;
	AttractorModel<Vector, Value>	_attModel;
	
public:
	Attractor(IAttractorUpdate<Matrix, Vector, Value>* attUpdate) : 
		_updateStrategy(attUpdate){
	}

	Attractor(IAttractorUpdate<Matrix, Vector, Value>* attUpdate, const Vector& position) : 
		_updateStrategy(attUpdate), _attModel(position) {
	}

	void updateAttractor(Camera<Matrix, Vector, Value>* camera, GLFWInput* input){
		_updateStrategy->updateAttractor(&_attModel, camera, input);
	}

	void setUpdateStrategy(std::shared_ptr<IAttractorUpdate<Matrix, Vector, Value>> attUpdate){
		_updateStrategy.swap(attUpdate);
	}

	Vector getAttractorPos() const{
		return _attModel.getPosition();
	}
	
	void incrementDepth(const float inc){
		_attModel.incrementDepth(inc);
	}
	
	float getDepth() const{
		return _attModel.getDepth();
	}
	
};


#endif