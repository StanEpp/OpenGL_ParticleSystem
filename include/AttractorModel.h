#ifndef _ATTRACTORMODEL_
#define _ATTRACTORMODEL_

template <class Vector, class Value>
class AttractorModel{
private:
	Vector	_position;
	float 	_attDepth; //The z-Coordinate of the attractor
	
public:

	AttractorModel() = default;

	AttractorModel(const Vector& position) : 
		_position(position) {}

	Vector	getPosition() const{
		return _position;
	}

	//Override me
	virtual void setPosition(const Vector& position){
		_position = position;
	}

	void incrementDepth(const float inc){
		_attDepth += inc;
		if(_attDepth < 0){
			_attDepth = 0;
		} else if (_attDepth > 1000){
			_attDepth = 1000;
		}
	}

	float getDepth() const{
		return _attDepth;
	}

};

#endif