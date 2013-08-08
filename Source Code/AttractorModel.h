#ifndef _ATTRACTORMODEL_
#define _ATTRACTORMODEL_

template <class Vector, class Value>
class AttractorModel{
private:
	Vector	_position;
	
public:
	AttractorModel() : _position(){}
	~AttractorModel(){}

	//AttractorModel(Value x, Value y, Value z);

	AttractorModel(Vector position){
		_position = position;
	}

	Vector	getPosition(){
		return _position;
	}

	//Override me
	//virtual void setPosition(Value x, Value y, Value z) = 0;

	//Override me
	virtual void setPosition(Vector position){
		_position = position;
	}

};

#endif