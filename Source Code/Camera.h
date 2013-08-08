#ifndef _CAMERA_
#define _CAMERA_

#include "CameraModel.h"
#include "ICameraRender.h"
#include <boost\shared_ptr.hpp>

template <class Matrix, class Vector, class Value>
class Camera{
private:
	boost::shared_ptr<ICameraRender<Matrix, Vector, Value> >	_cameraRender;
	boost::shared_ptr<CameraModel<Matrix, Vector, Value> >		_cameraModel;

public:
	Camera() : _cameraModel(new CameraModel<Matrix, Vector, Value>()){}

	Camera(ICameraRender<Matrix, Vector, Value>* cameraRender) : 
									_cameraModel(new CameraModel<Matrix, Vector, Value>()){
		_cameraRender = boost::shared_ptr<ICameraRender<Matrix, Vector, Value> >(cameraRender);
	}

	~Camera(){}

	void updateCamera(){
		_cameraRender->updateCamera(_cameraModel.get());
	}

	void setCameraRender(ICameraRender<Matrix, Vector, Value>* cameraRender){
		_cameraRender.swap(boost::shared_ptr<ICameraRender<Matrix, Vector, Value> >(cameraRender));
	}

	Matrix		getViewMatrix(){
		return _cameraModel->getViewMatrix();
	}

	Matrix		getRotMatrix(){
		return	_cameraModel->getRotMatrix();
	}

	Vector		getPosition(){
		return _cameraModel->getPosition();
	}
	Vector		getLookToVector(){
		return _cameraModel->getLookToVector();
	}

	Vector		getUpVector(){
		return _cameraModel->getUpVector();
	}

	Vector		getRotationVector(){
		return _cameraModel->getRotationVector();
	}

	Value		getPitch(){
		return _cameraModel->getPitch();
	}

	Value		getYaw(){
		return _cameraModel->getYaw();
	}

	Value		getRoll(){
		return _cameraModel->getRoll();
	}

	void		setPosition(Vector position){
		_cameraModel->setPosition(position);
	}

	void		setLookToVector(Vector lookTo){
		_cameraModel->setLookToVector(lookTo);
	}

	void		setUpVector(Vector up){
		_cameraModel->setUpVector(up);
	}

	void		setAddPitch(Value pitch){
		_cameraModel->setAddPitch(pitch);
	}

	void		setAddYaw(Value yaw){
		_cameraModel->setAddYaw(yaw);
	}

	void		setAddRoll(Value roll){
		_cameraModel->setAddRoll(roll);
	}

	void		setAddXPos(Value addXPos){
		_cameraModel->setAddXPos(addXPos);
	}

	void		setAddYPos(Value addYPos){
		_cameraModel->setAddYPos(addYPos);
	}

	void		setAddZPos(Value addZPos){
		_cameraModel->setAddZPos(addZPos);
	}

	void		setPitch(Value pitch){
		_cameraModel->setPitch(pitch);
	}

	void		setYaw(Value yaw){
		_cameraModel->setYaw(yaw);
	}

	void		setRoll(Value roll){
		_cameraModel->setRoll(roll);
	}

};


#endif _CAMERA_