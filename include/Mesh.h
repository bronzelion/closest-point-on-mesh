#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>
#include "Vector.h"
#include "Face.h"
#include "ply.h"

using namespace std;
using std::vector;
using namespace math;

class Mesh{

private:
	vector<Vec3f> vertex_list;
	vector<Face> face_list;
	std::vector<set <int> > edgeList;

public:
	int nverts,nfaces;

	Mesh(){};
	Mesh(const Mesh &obj);	
	~Mesh(){};
	
	/*Getters and Setters for Mesh atributes*/
	inline vector<Vec3f> get_vertexList(){
		return vertex_list;
	}

	inline vector<Face> get_faceList(){
		return face_list;
	}

	inline std::vector<set <int> > get_edgeList(){
		return edgeList;
	}
	inline void set_edgeList(std::vector<set <int> > e){
		edgeList = e;
	}

	bool readPly(char* name);
};

