#ifndef _CAMERAMODEL_
#define _CAMERAMODEL_

template <class Matrix, class Vector, class Value>
class CameraModel {
private:
	Matrix		_viewMatrix, _rotMatrix;
	Vector		_lookTo, _up, _position;
	Value		_pitch, _yaw, _roll;
	Value		_addPitch, _addYaw, _addRoll;
	Value		_addXPos, _addYPos, _addZPos;

public:
	CameraModel() : _viewMatrix(), _rotMatrix(), _lookTo(), _up(), _position(){}
	~CameraModel(){}

	Matrix		getViewMatrix(){	return _viewMatrix; }
	Matrix		getRotMatrix(){		return _rotMatrix; }

	Vector		getPosition(){		return _position; }
	Vector		getLookToVector(){	return _lookTo; }
	Vector		getUpVector(){		return _up; }
	Vector		getRotation(){		return Vector(_pitch, _yaw, _roll); }

	Value		getPitch(){			return _pitch; }
	Value		getYaw(){			return _yaw; }
	Value		getRoll(){			return _roll; }

	Value		getAddPitch(){			return _addPitch; }
	Value		getAddYaw(){			return _addYaw; }
	Value		getAddRoll(){			return _addRoll; }

	Value		getAddXPos(){			return _addXPos; }
	Value		getAddYPos(){			return _addYPos; }
	Value		getAddZPos(){			return _addZPos; }

	void		setViewMatrix(Matrix mat){
					_viewMatrix = mat;
	}

	void		setRotMatrix(Matrix mat){
					_rotMatrix = mat;
	}

	void		setPosition(Vector position){
					_position = position;
	}

	void		setLookToVector(Vector lookTo){
					_lookTo = lookTo;
	}

	void		setUpVector(Vector up){
					_up = up;
	}

	void		setPitch(Value pitch){
					_pitch = pitch;
	}

	void		setYaw(Value yaw){
					_yaw = yaw;
	}

	void		setRoll(Value roll){
					_roll = roll;
	}

	void		setAddPitch(Value AddPitch){
					_addPitch = AddPitch;
	}

	void		setAddYaw(Value AddYaw){
					_addYaw = AddYaw;
	}

	void		setAddRoll(Value AddRoll){
					_addRoll = AddRoll;
	}

	void		setAddXPos(Value AddXPos){
					_addXPos = AddXPos;
	}

	void		setAddYPos(Value AddYPos){
					_addYPos = AddYPos;
	}

	void		setAddZPos(Value AddZPos){
					_addZPos = AddZPos;
	}

};


#endif