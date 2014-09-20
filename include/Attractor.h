#ifndef _ATTRACTOR_
#define _ATTRACTOR_

#define GLM_FORCE_RADIANS

#include <boost\shared_ptr.hpp>

#include <glm\glm.hpp>

#include "Classes.h"
#include "AttractorModel.h"
#include "IAttractorUpdate.h"
#include "Camera.h"
#include "GLFWInput.h"

template <class Matrix, class Vector, class Value>
class Attractor{
private:
	boost::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> >	_updateStrategy;
	boost::shared_ptr<AttractorModel<Vector, Value> >						_attModel;
	
public:
	Attractor(IAttractorUpdate<Matrix, Vector, Value>* attUpdate) : _updateStrategy(attUpdate){
		_attModel = boost::shared_ptr<AttractorModel<Vector, Value> >(new AttractorModel<Vector, Value>());
	}

	Attractor(IAttractorUpdate<Matrix, Vector, Value>* attUpdate, Value x, Value y, Value z) : _updateStrategy(attUpdate) {
		_attModel = boost::shared_ptr<AttractorModel<Vector, Value> >(new AttractorModel<Vector, Value>(x, y, z));
	}

	Attractor(IAttractorUpdate<Matrix, Vector, Value>* attUpdate, Vector position) : _updateStrategy(attUpdate) {
		_attModel = boost::shared_ptr<AttractorModel<Vector, Value> >(new AttractorModel<Vector, Value>(position));
	}

	~Attractor(){}

	void updateAttractor(Camera<Matrix, Vector, Value>* camera, GLFWInput* input){
		_updateStrategy->updateAttractor(_attModel.get(), camera, input);
	}

	void setUpdateStrategy(boost::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> > attUpdate){
		boost::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> > ptr = boost::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> >(attUpdate);
		_updateStrategy.swap(ptr);
	}

	Vector getAttractorPos(){
		return _attModel->getPosition();
	}
	
	void incrementDepth(float inc){
		_attModel->incrementDepth(inc);
	}
	
	float getDepth(){
		return _attModel->getDepth();
	}
	
};


#endif