#ifndef _ATTRACTOR_
#define _ATTRACTOR_

#include <boost\shared_ptr.hpp>
#include "IAttractorUpdate.h"
#include "AttractorModel.h"
#include <glm\glm.hpp>
#include "Camera.h"
#include "GLFWInput.h"

template <class Matrix, class Vector, class Value>
class Attractor{
private:
	boost::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> >	_updateStrategy;
	boost::shared_ptr<AttractorModel<Vector, Value> >			_attModel;

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

	void updateAttractor(Camera<Matrix, Vector, Value>* camera, Input* input){
		_updateStrategy->updateAttractor(_attModel.get(), camera, input);
	}

	void setUpdateStrategy(IAttractorUpdate<Matrix, Vector, Value>* attUpdate){
		_updateStrategy.swap(boost::shared_ptr<IAttractorUpdate<Matrix, Vector, Value> >(attUpdate));
	}

	Vector getAttractorPos(){
		return _attModel->getPosition();
	}
	
};


#endif