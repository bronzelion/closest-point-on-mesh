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

public:
	Mesh(){};
	Mesh(const Mesh &obj);	
	~Mesh(){};
	
	vector<Vec3f> vertex_list;
	vector<Face> face_list;
	std::vector<set <int> > edgeList;

	vector<Vec3f> get_vlist();
	vector<Face> get_flist();
	int nverts,nfaces;

	bool readPly(char* name);
	int** getEdgeList();


};

