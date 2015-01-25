#ifndef _CAMERA_
#define _CAMERA_

#include <memory>

#include "CameraModel.h"
#include "ICameraRender.h"

template <class Matrix, class Vector, class Value>
class Camera{
private:
	std::shared_ptr<ICameraRender<Matrix, Vector, Value> >	_cameraRender;
	std::shared_ptr<CameraModel<Matrix, Vector, Value> >	_cameraModel;

public:
	Camera() : _cameraModel(new CameraModel<Matrix, Vector, Value>()){}

	Camera(ICameraRender<Matrix, Vector, Value>* cameraRender) : 
		_cameraRender(cameraRender), _cameraModel(new CameraModel<Matrix, Vector, Value>()) {}

	void updateCamera(){
		_cameraRender->updateCamera(_cameraModel.get());
	}

	void setCameraRender(std::shared_ptr<ICameraRender<Matrix, Vector, Value>> cameraRender){
		_cameraRender.swap(cameraRender);
	}

	Matrix	getViewMatrix() const{
		return _cameraModel->getViewMatrix();
	}

	Matrix	getRotMatrix() const{
		return	_cameraModel->getRotMatrix();
	}

	Vector	getPosition() const{
		return _cameraModel->getPosition();
	}
	Vector	getLookToVector() const{
		return _cameraModel->getLookToVector();
	}

	Vector	getUpVector() const{
		return _cameraModel->getUpVector();
	}
	
	Vector	getRightVector() const{
		return _cameraModel->getRightVector();
	}

	Vector	getRotationVector() const{
		return _cameraModel->getRotationVector();
	}

	Value	getPitch() const{
		return _cameraModel->getPitch();
	}

	Value	getYaw() const{
		return _cameraModel->getYaw();
	}

	Value	getRoll() const{
		return _cameraModel->getRoll();
	}

	void	setPosition(const Vector& position){
		_cameraModel->setPosition(position);
	}

	void	setLookToVector(const Vector& lookTo){
		_cameraModel->setLookToVector(lookTo);
	}

	void	setUpVector(const Vector& up){
		_cameraModel->setUpVector(up);
	}
	
	void	setRightVector(const Vector& right){
		_cameraModel->setRightVector(right);
	}

	void	setAddPitch(const Value& pitch){
		_cameraModel->setAddPitch(pitch);
	}

	void	setAddYaw(const Value& yaw){
		_cameraModel->setAddYaw(yaw);
	}

	void	setAddRoll(const Value& roll){
		_cameraModel->setAddRoll(roll);
	}

	void	setAddXPos(const Value& addXPos){
		_cameraModel->setAddXPos(addXPos);
	}

	void	setAddYPos(const Value& addYPos){
		_cameraModel->setAddYPos(addYPos);
	}

	void	setAddZPos(const Value& addZPos){
		_cameraModel->setAddZPos(addZPos);
	}

	void	setPitch(const Value& pitch){
		_cameraModel->setPitch(pitch);
	}

	void	setYaw(const Value& yaw){
		_cameraModel->setYaw(yaw);
	}

	void	setRoll(const Value& roll){
		_cameraModel->setRoll(roll);
	}

};


#endif