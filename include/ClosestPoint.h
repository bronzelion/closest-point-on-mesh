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
private:
	Mesh mesh;

public:
	/*Filtered vertices and faces within Maxradius*/
	set<int> vlist_in_sphere;
	set<int> edge_in_sphere;	

	ClosestPoint(const Mesh &m){
		mesh = Mesh(m);	
		/*Ply give vertex and face list, make edge list*/		
		makeEdgeList();	
	};

	~ClosestPoint(){};

	/*Prepare Adjacency List for edges from Vertex and Face list*/
	void makeEdgeList();

	/*Overload the call () operator */
	Vec3f operator ()(const Vec3f &point, float maxRadius);
	
	/*Minimum distance of Query Point from 	all vertices*/
	pair<float,Vec3f> minVertexDistance(const Vec3f &point, float maxRadius);

	/*Closest point on an edge, from all edges in  the mesh*/
	pair<float,Vec3f> minEdgeDistance(const Vec3f &point, float maxRadius);

	/*Closest point on a face, from all faces in  the mesh*/
	pair<float,Vec3f> minFaceDistance(const Vec3f &point, float maxRadius);

	inline Mesh getMesh(){
		return mesh;
	}

	/*Fetch only the faces which contain vertices/edges within MaxRadius*/
	vector<Face> fetchFacesinSphere();
	
};