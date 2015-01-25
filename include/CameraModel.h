#ifndef _CAMERAMODEL_
#define _CAMERAMODEL_

template <class Matrix, class Vector, class Value>
class CameraModel {
private:
	Matrix		_viewMatrix, _rotMatrix;
	Vector		_lookTo, _up, _right, _position;
	Value		_pitch, _yaw, _roll;
	Value		_addPitch, _addYaw, _addRoll;
	Value		_addXPos, _addYPos, _addZPos;

public:

	Matrix	getViewMatrix() const{	return _viewMatrix; }
	Matrix	getRotMatrix() const{	return _rotMatrix; }

	Vector	getPosition() const{	return _position; }
	Vector	getLookToVector() const{	return _lookTo; }
	Vector	getUpVector() const{	return _up; }
	Vector	getRightVector() const{	return _right; }
	Vector	getRotation() const{	return Vector(_pitch, _yaw, _roll); }

	Value	getPitch() const{	return _pitch; }
	Value	getYaw() const{	return _yaw; }
	Value	getRoll() const{	return _roll; }

	Value	getAddPitch() const{	return _addPitch; }
	Value	getAddYaw() const{	return _addYaw; }
	Value	getAddRoll() const{	return _addRoll; }

	Value	getAddXPos() const{	return _addXPos; }
	Value	getAddYPos() const{	return _addYPos; }
	Value	getAddZPos() const{	return _addZPos; }

	void	setViewMatrix(const Matrix& mat){
		_viewMatrix = mat;
	}

	void	setRotMatrix(const Matrix& mat){
		_rotMatrix = mat;
	}

	void	setPosition(const Vector& position){
		_position = position;
	}

	void	setLookToVector(const Vector& lookTo){
		_lookTo = lookTo;
	}

	void	setUpVector(const Vector& up){
		_up = up;
	}
	
	void	setRightVector(const Vector& right){
		_right = right;
	}

	void	setPitch(const Value& pitch){
		_pitch = pitch;
	}

	void	setYaw(const Value& yaw){
		_yaw = yaw;
	}

	void	setRoll(const Value& roll){
		_roll = roll;
	}

	void	setAddPitch(const Value& AddPitch){
		_addPitch = AddPitch;
	}

	void	setAddYaw(const Value& AddYaw){
		_addYaw = AddYaw;
	}

	void	setAddRoll(const Value& AddRoll){
		_addRoll = AddRoll;
	}

	void	setAddXPos(const Value& AddXPos){
		_addXPos = AddXPos;
	}

	void	setAddYPos(const Value& AddYPos){
		_addYPos = AddYPos;
	}

	void	setAddZPos(const Value& AddZPos){
		_addZPos = AddZPos;
	}

};


#endif