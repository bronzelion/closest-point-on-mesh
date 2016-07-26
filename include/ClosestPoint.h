#pragma once
#include "Mesh.h"
#include "Vector.h"
#include <math.h>
#include <limits>

#define INFINITY_FLOAT std::numeric_limits<float>::infinity();
#define INFINITY_INT std::numeric_limits<int>::infinity();
using namespace math;
using namespace std;

class ClosestPoint{

public:
	Mesh mesh;

	Vec3f min_vertex;
	Vec3f min_vertex_idx;

	set<int> vlist_in_sphere;
	set<int> edge_in_sphere;
	

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

	vector<Face> fetchFacesinSphere();
	
};