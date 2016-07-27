#pragma once

#include <iostream>
#include "Vector.h"
using namespace math;

class Face{

public:
	Face(){}
	~Face(){}

	unsigned char nverts;		/*Vertex count in a face*/
	Vec3f v1,v2,v3;				/*Vertices which make the face*/
	int *vid;					/*Vertex Index which form the face*/
	int id;						/*Face ID*/
};