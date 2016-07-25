#include "ClosestPoint.h"
#include <utility>
#include <set>
#include <algorithm>

using namespace std;

bool comp(const std::pair<float,Vec3f>& firstElem, const std::pair<float,Vec3f>& secondElem){
  	return firstElem.first < secondElem.first;
}


Vec3f ClosestPoint::operator () (const Vec3f &queryPoint, float maxRadius){
	/* Closest Point on Mesh, can either be a vertex, point on edge,
	 or a point in the interior of a trianlge. 
	 Returns: min(Vertex,EdgePoint, FacePoint)
	*/
	vector< pair <float,Vec3f> > closestPoints;

	closestPoints.push_back( minVertexDistance(queryPoint,maxRadius));
	closestPoints.push_back( minEdgeDistance(queryPoint,maxRadius));
	closestPoints.push_back( minFaceDistance(queryPoint,maxRadius));
	
	std::sort(closestPoints.begin(), closestPoints.end(),comp);
	Vec3f result = closestPoints[0].second;

	if (result == NULL)
		cout<< "No Closest Point on mesh found within given maxRadius";
	else
		cout << "ClOSEST POINT : "<< result.x<< " "<< result.y<< " "<< result.z;

	return result;
}

pair<float,Vec3f> ClosestPoint::minFaceDistance(const Vec3f &point, float maxRadius){

	double dmin = INFINITY;	
	Vec3f min_face = NULL;
	int faceID;

	for(int i = 0; i<mesh.face_list.size(); i++){
		Vec3f v0 = mesh.face_list[i].v1;
		Vec3f v1 = mesh.face_list[i].v2;
		Vec3f v2 = mesh.face_list[i].v3;

		Vec3f d  = (v0 - point);
		Vec3f e0 = v1 - v0;
		Vec3f e1 = v2 - v0;

		e0.normalize();				
		e1.normalize();

		Vec3f a = e0.dot(e0);
		Vec3f b = e0.dot(e1);
		Vec3f c = e1.dot(e1);
		//Vec3f d = e0.dot(d);
		Vec3f e = -e1.dot(d);
		Vec3f f = d.dot(d);

		Vec3f detv = (a*c) - (b*b); 
		Vec3f sv   = (b*e) - (c*d);
		Vec3f tv   = (b*d) - (a*e);

		float det = detv.length(); 
		float s   = sv.length();
		float t   = tv.length();

		if((s+t <= det) && (s>=0) && (t>=0)){			
			float inv = 1/det;
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
	cout<<"Distance " << dmin << "faceID "<<faceID<<endl ;
	return std::pair<float,Vec3f>(dmin,min_face);
}

float dist_lineseg_to_point(Vec3f x0, Vec3f x1, const Vec3f p, Vec3f &result){
	// Ditance of point P from Edge X1,X2
	//Vec3f result;
	//float result;

	Vec3f v	= x1-x0;
	Vec3f w	= p-x0;
	//cout << "Before" << v.x << endl;
	float c1 = w.dot(v);
	float c2 = v.dot(v);
	if (!(c1<0) && !(c2 <=c1)){
		float b = c1/c2;
		result = x0+ v*b;
		//cout << "X0 "<< x0.x<<"V "<<v.x << "B " << b<<endl;
		//cout << "result" << result.x<<endl;

		return result.distance(p);
	}
	return INFINITY;
}

pair<float,Vec3f> ClosestPoint::minEdgeDistance(const Vec3f &point, float maxRadius){
//Vec3f ClosestPoint::minEdgeDistance(const Vec3f &point, float maxRadius){
	// Loop thorough Adj List, find the distance from point to every edge
	Vec3f minEdgePoint;
	Vec3f result;
	int v1,v2;
	float dmin = INFINITY;
	cout <<"In Point" << point.x<<endl;
	for (int i =0; i<mesh.edgeList.size(); i++){

		set <int> temp = mesh.edgeList[i];
		
		Vec3f x1 = mesh.vertex_list[i];
		

		for (set<int>::iterator j = temp.begin(); j != temp.end(); j++) {
   			int element = *j;
			Vec3f x2 = mesh.vertex_list[element];
			float d = dist_lineseg_to_point(x1,x2,point,result);
			if (d < (maxRadius) && d<dmin){
				dmin = d;
				minEdgePoint = result; 
				//cout << "After "<<point.x<<endl;
				v1 = i;
				v2 = element;
			}
		}
		
	}
	cout << "Closest Edge Point ";
	cout << minEdgePoint.x <<" "<< minEdgePoint.y <<" "<< minEdgePoint.z <<endl;
	cout << "Vertex containing edge "<<endl;
	cout << "V1 " << v1 << endl;//v1.x << " "<< v1.y<< " "<< v1.z<<endl;
	cout << "V2 " << v2<<endl;//v2.x << " "<< v2.y<< " "<< v2.z<<endl;
	cout << "Distance " << dmin<<endl;

	return pair<float,Vec3f> (dmin,minEdgePoint);
}

void ClosestPoint::getEdgeList(){
	int size = mesh.nverts;
	cout << "size"<<size<<endl;
	//std::vector<set <int> > edgeList;
	set <int> temp;

	for(int i = 0; i< size; i++)
		mesh.edgeList.push_back(temp);

//Loop through all faces, prepare adjacency list
	for (int i =0 ; i< mesh.face_list.size(); i++){		
  		int n = mesh.face_list[i].nverts;
  		std::vector<set <int> > :: iterator it;
  		//cout << "N" << n;
  		for(int j = 0; j< n;  j++){
			int idx1 = mesh.face_list[i].vid[j];
			int idx2 = mesh.face_list[i].vid[(j+1) % n];

			mesh.edgeList[idx1].insert(idx2);
					
  		}
	}
	for (int i =0; i< mesh.edgeList.size();i++){
		set <int> temp = mesh.edgeList[i];
		//cout <<"Edge "<< i << " ";
		for (set<int>::iterator j = temp.begin(); j != temp.end(); j++) {
   			int element = *j;
   			//cout << element << " ";
   		}
   		//cout<<endl;
	}
	
}
pair<float,Vec3f> ClosestPoint::minVertexDistance(const Vec3f &point, float maxRadius){
//Vec3f ClosestPoint::minVertexDistance(Vec3f &point, float maxRadius){
	float dmin = std::numeric_limits<float>::infinity();
	min_vertex_idx = std::numeric_limits<int>::infinity();

	cout << "Infinity "<<dmin<<endl;
	Vec3f closest;
	float d;
	for(int i=0; i< mesh.vertex_list.size();i++){
		d = mesh.vertex_list[i].distance(point);		
		if (d<dmin && d<= maxRadius){
			dmin = d;
			closest = mesh.vertex_list[i];
			min_vertex_idx = i;
			vlist_in_sphere.push_back(closest);
		}	
	}
	cout << "Closest Vertex to Query Point ";
	cout << closest.x <<" "<< closest.y <<" "<< closest.z <<endl;
	cout << "Distance " << dmin<< endl;
	

	/*if (dmin == std::numeric_limits<float>::infinity())
		return NULL;*/
	return std::pair<float,Vec3f>(dmin,closest);
	//return closest;

}

int main(int argc, char** argv){
	Mesh mesh;
	
	//char* filename = "./bunny.ply";
	char* filename = "data/bunny.ply";

	if (!mesh.readPly(filename)){
		cout <<"Failed to read ply file"<<endl;
		return 1;
	}
	ClosestPoint cp = ClosestPoint(mesh);
	cp.getEdgeList();

	cp(Vec3f(-0.0083935,0.065325,-0.0472749),0.01);
	cp(Vec3f(-0.0083935,0.23,-0.0472749),0.10);
	cp(	Vec3f(10.0,10.0,12.0),0.23);
	//Vec3f point = Vec3f(10.0,10.0,12.0);
	//Vec3f point = Vec3f(-0.037,0.127,0.004);
	//Vec3f point =(-0.0255083,0.112568,0.0366767);
	/*Vec3f point = Vec3f(-0.0083935,0.065325,-0.0472749);
	//Vec3f min_vertex = cp.minVertexDistance(point,10);
	std::pair<float,Vec3f> min_vertex = cp.minVertexDistance(point,10);
	cout << "Pair"<< min_vertex.first << " "<<min_vertex.second.x<<endl;

	
	Vec3f min_edge_point = cp.minEdgeDistance(point,0.01);
	cout <<"Min Edge Point " << min_edge_point.x << " " <<min_edge_point.y << " " << min_edge_point.z<<endl;

	Vec3f min_face_point = cp.minFaceDistance(point,1.0);
	cout <<"Min face Point " << min_face_point.x << " " <<min_face_point.y << " " << min_face_point.z;	*/
	return 0;
}
