#pragma once
#include "Mesh.h"
#include "Vector.h"
#include <math.h>
#include <limits>

#define INFINITY std::numeric_limits<float>::infinity();

using namespace math;
using namespace std;

class ClosestPoint{

public:
	Mesh mesh;

	Vec3f min_vertex;
	Vec3f min_vertex_idx;

	std::vector<Vec3f> vlist_in_sphere;
	

	ClosestPoint(Mesh &m){
		mesh = Mesh(m);
		//Mesh mesh = m;
	};
	~ClosestPoint(){};
	void getEdgeList();

	Vec3f operator ()(const Vec3f &point, float maxRadius);
	//Vec3f minVertexDistance(Vec3f &point, float maxRadius);
	pair<float,Vec3f> minVertexDistance(const Vec3f &point, float maxRadius);
	pair<float,Vec3f> minEdgeDistance(const Vec3f &point, float maxRadius);
	pair<float,Vec3f> minFaceDistance(const Vec3f &point, float maxRadius);
	
};