#ifndef _IATTRACTORUPDATE_
#define _IATTRACTORUPDATE_

#include "Classes.h"

template <class Matrix, class Vector, class Value>
class IAttractorUpdate{
public:
	~IAttractorUpdate(){}

	//Override me
	virtual void updateAttractor(AttractorModel<Vector, Value>* attModel, Camera<Matrix, Vector, Value>* camera, Input* Input) = 0;
};


#endif