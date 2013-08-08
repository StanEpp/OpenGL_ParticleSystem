#ifndef _ICAMERARENDER_
#define _ICAMERARENDER_

template <class Matrix, class Vector, class Value> class CameraModel;

template <class Matrix, class Vector, class Value>
class ICameraRender{
public:
	virtual ~ICameraRender(){}

	//Override me
	virtual void updateCamera(CameraModel<Matrix, Vector, Value>* camera) = 0;
};



#endif