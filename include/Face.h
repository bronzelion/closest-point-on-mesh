#pragma once

#include <iostream>
#include "Vector.h"
using namespace math;

class Face{

public:
	Face(){}
	~Face(){}

	unsigned char nverts;
	Vec3f v1,v2,v3;
	int *vid;
	int id;	

};