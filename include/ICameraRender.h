#ifndef _ICAMERARENDER_
#define _ICAMERARENDER_

#include "Classes.h"

template <class Matrix, class Vector, class Value>
class ICameraRender{
public:
	virtual ~ICameraRender(){}

	//Override me
	virtual void updateCamera(CameraModel<Matrix, Vector, Value>* camera) = 0;
};



#endif