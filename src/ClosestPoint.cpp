#include "ClosestPoint.h"
#include <utility>
#include <set>
#include <algorithm>
#include <time.h>

using namespace std;

/*Comparator to sort the pair of Closest Points by distance*/
bool comp(const std::pair<float,Vec3f>& firstElem, 
				const std::pair<float,Vec3f>& secondElem){
  	return firstElem.first < secondElem.first;
}

/*T*/
Vec3f ClosestPoint::operator () (const Vec3f &queryPoint, float maxRadius){
	/* Closest Point on Mesh, can either be a vertex, point on edge,
	 or a point in the interior of a trianlge. 
	 Returns: min(dVertex,dEdgePoint, dFacePoint)
	*/
	cout<< "Query Point ";
	cout << queryPoint.x <<" "<< queryPoint.y << " "<< queryPoint.z;
	cout <<", Max Radius " << maxRadius<<endl;

	vector< pair <float,Vec3f> > closestPoints;
	
	closestPoints.push_back( minVertexDistance(queryPoint,maxRadius));			
	closestPoints.push_back( minEdgeDistance(queryPoint,maxRadius));		
	closestPoints.push_back( minFaceDistance(queryPoint,maxRadius));

	std::sort(closestPoints.begin(), closestPoints.end(),comp);
	Vec3f result = closestPoints[0].second;

	cout <<"-----------------------------------------------------------------"<<endl;
	if (result == NULL)
		cout<< "No Closest Point on mesh found within given maxRadius"<<endl;
	else
		cout <<"CLOSEST POINT ON MESH: "<< result.x<< " "<< result.y<< " "<< result.z<<endl;
	cout <<"-----------------------------------------------------------------"<<endl;
	cout<<endl;
	return result;
}


pair<float,Vec3f> ClosestPoint::minFaceDistance(const Vec3f &point, float maxRadius){

	double dmin = INFINITY;	
	Vec3f min_face = NULL;
	int faceID;
	std::vector<Face> faces_in_sphere;	
	faces_in_sphere = mesh.get_faceList();		
	
	for(int i = 0; i<faces_in_sphere.size(); i++){
		Vec3f v0 = faces_in_sphere[i].v1;
		Vec3f v1 = faces_in_sphere[i].v2;
		Vec3f v2 = faces_in_sphere[i].v3;
		
		Vec3f d  = (v0 - point);
		Vec3f e0 = v1 - v0;
		Vec3f e1 = v2 - v0;

		e0.normalize();				
		e1.normalize();

		Vec3f a = e0.dot(e0);
		Vec3f b = e0.dot(e1);
		Vec3f c = e1.dot(e1);		
		Vec3f e = -e1.dot(d);
		Vec3f f = d.dot(d);

		Vec3f detv = (a*c) - (b*b); 
		Vec3f sv   = (b*e) - (c*d);
		Vec3f tv   = (b*d) - (a*e);

		float det = detv.length(); 
		float s   = sv.length();
		float t   = tv.length();

		if((s+t <= det) && (s>=0) && (t>=0)){			
			double inv = 1/det;
			s *= inv;
			t*= inv;
			Vec3f result = v0 + e0*s + e1*t;
			float d = result.distance(point);
			if (d < dmin && d <= maxRadius){
				dmin = d;
				min_face = result;
				faceID = i;
			}
		}
	}
	cout << "Closest Point on Face to Query Point ";
	cout<<min_face.x << " "<< min_face.y << " "<< min_face.z;
	cout<<", Distance " << dmin <<endl ;	
	return std::pair<float,Vec3f>(dmin,min_face);
}

/*Helper function  to Ditance of point P from Edge (X0,X1)*/
float dist_lineseg_to_point(Vec3f x0, Vec3f x1, const Vec3f p, Vec3f &result){	

	Vec3f v	= x1-x0;
	Vec3f w	= p-x0;
	
	float c1 = w.dot(v);
	float c2 = v.dot(v);

	if (!(c1<0) && !(c2 <=c1)){
		float b = c1/c2;
		result = x0+ v*b;
		return result.distance(p);
	}
	return INFINITY;
}

/* Loop thorough Adj List, find the distance from point to every edge*/
pair<float,Vec3f> ClosestPoint::minEdgeDistance(const Vec3f &point,float maxRadius){

	Vec3f minEdgePoint;
	Vec3f result;
	int v1,v2;
	float dmin = INFINITY;

	vector<Vec3f> vertex_list = mesh.get_vertexList();
	std::vector<set <int> > edgeList = mesh.get_edgeList();
	
	for (int i =0; i<edgeList.size(); i++){

		set <int> temp = edgeList[i];		
		Vec3f x1 = vertex_list[i];		

		for (set<int>::iterator j = temp.begin(); j != temp.end(); j++) {
   			int element = *j;
			Vec3f x2 = vertex_list[element];
			float d = dist_lineseg_to_point(x1,x2,point,result);
			if (d < (maxRadius) && d<dmin){
				dmin = d;
				minEdgePoint = result; 
				edge_in_sphere.insert(i);
				edge_in_sphere.insert(element);
				//cout << "After "<<point.x<<endl;
				v1 = i;
				v2 = element;
			}
		}		
	}

	cout << "Closest Point on Edge to Query Point ";
	cout << minEdgePoint.x <<" "<< minEdgePoint.y <<" "<< minEdgePoint.z;
	cout << ", Distance " << dmin<<endl;
	return pair<float,Vec3f> (dmin,minEdgePoint);
}

pair<float,Vec3f> ClosestPoint::minVertexDistance(const Vec3f &point, float maxRadius){

	float dmin = INFINITY_FLOAT;	
	Vec3f closest;
	float d;
	vector<Vec3f> vertex_list = mesh.get_vertexList();

	for(int i=0; i< vertex_list.size();i++){
		d = vertex_list[i].distance(point);		
		if (d<dmin && d<= maxRadius){
			dmin = d;
			closest = vertex_list[i];			
			vlist_in_sphere.insert(i);
		}	
	}
	cout << "Closest Vertex to Query Point ";
	cout << closest.x <<" "<< closest.y <<" "<< closest.z;
	cout << ", Distance " << dmin<< endl;

	return std::pair<float,Vec3f>(dmin,closest);
}

void ClosestPoint::makeEdgeList(){
	int size = mesh.nverts;	
	set <int> temp;
	std::vector<set <int> > edgeList;
	vector<Face> face_list = mesh.get_faceList();

	for(int i = 0; i< size; i++)
		edgeList.push_back(temp);

	//Loop through all faces, prepare adjacency list
	for (int i =0 ; i< face_list.size(); i++){		
  		int n = face_list[i].nverts;
  		std::vector<set <int> > :: iterator it;

		/*Each face stores vertex ID list, convert it to 3 edges*/  		
  		for(int j = 0; j< n;  j++){
			int idx1 = face_list[i].vid[j];
			int idx2 = face_list[i].vid[(j+1) % n];

			edgeList[idx1].insert(idx2);					
  		}
	}
	mesh.set_edgeList(edgeList);	
}

/*Find distance of point on face, of only those faces which have
vertices or edges within the maximum search radius from queryPoint*/

std::vector<Face> ClosestPoint::fetchFacesinSphere(){
	
	std::set <int> valid_verts;
	
	std::vector<Face> result;
	vector<Face> face_list = mesh.get_faceList();

	set_union(vlist_in_sphere.begin(),vlist_in_sphere.end(),
						edge_in_sphere.begin(), edge_in_sphere.end(),
                  		std::inserter(valid_verts,valid_verts.begin()));

	for (int i =0; i< face_list.size(); i++){	
		/*Create set of current face vertices*/	
		std::set<int> temp;
		for (int j =0; j<3; j++ ){
			temp.insert(face_list[i].vid[j]);
		}

   		/*Check interesction with Bounding Sphere*/
   		set<int> intersect;
		set_intersection(temp.begin(),temp.end(),valid_verts.begin(),
						valid_verts.end(),
                  		std::inserter(intersect,intersect.begin()));
   		if (intersect.size()>0)
   			result.push_back(face_list[i]);   		
	}	
	return result;
}
