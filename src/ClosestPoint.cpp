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
		cout << "CLOSEST POINT ON MESH: "<< result.x<< " "<< result.y<< " "<< result.z<<endl;
	cout <<"-----------------------------------------------------------------"<<endl;
	cout<<endl;
	return result;
}

pair<float,Vec3f> ClosestPoint::minFaceDistance(const Vec3f &point, float maxRadius){

	double dmin = INFINITY;	
	Vec3f min_face = NULL;
	int faceID;

	/* Find only the faces which have edges/vertices within search radius*/
	std::vector<Face> faces_in_sphere = fetchFacesinSphere();
	//cout << "Searching only : " << faces_in_sphere.size()<<" faces"<<endl;

	for(int i = 0; i<faces_in_sphere.size(); i++){
		Vec3f v0 = faces_in_sphere[i].v1;
		Vec3f v1 = faces_in_sphere[i].v2;
		Vec3f v2 = faces_in_sphere[i].v3;
		//cout << "Face ID " << faces_in_sphere[i].id<< "Vertex 0 "<< faces_in_sphere[i].vid[0]<<endl;
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
	cout << "Closest Point on Face to Query Point ";
	cout<<min_face.x << " "<< min_face.y << " "<< min_face.z;
	cout<<", Distance " << dmin <<endl ;
	return std::pair<float,Vec3f>(dmin,min_face);
}

float dist_lineseg_to_point(Vec3f x0, Vec3f x1, const Vec3f p, Vec3f &result){
	// Ditance of point P from Edge (X0,X1)

	Vec3f v	= x1-x0;
	Vec3f w	= p-x0;
	//cout << "Before" << v.x << endl;
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
pair<float,Vec3f> ClosestPoint::minEdgeDistance(const Vec3f &point, float maxRadius){

	Vec3f minEdgePoint;
	Vec3f result;
	int v1,v2;
	float dmin = INFINITY;
	
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
	//cout << "Vertex containing edge "<<endl;
	//cout << "V1 " << v1 << endl;//v1.x << " "<< v1.y<< " "<< v1.z<<endl;
	//cout << "V2 " << v2<<endl;//v2.x << " "<< v2.y<< " "<< v2.z<<endl;
	cout << ", Distance " << dmin<<endl;
	return pair<float,Vec3f> (dmin,minEdgePoint);
}

void ClosestPoint::getEdgeList(){
	int size = mesh.nverts;	
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
}


pair<float,Vec3f> ClosestPoint::minVertexDistance(const Vec3f &point, float maxRadius){
//Vec3f ClosestPoint::minVertexDistance(Vec3f &point, float maxRadius){
	float dmin = INFINITY_FLOAT;
	min_vertex_idx = INFINITY_INT;
	Vec3f closest;
	float d;
	for(int i=0; i< mesh.vertex_list.size();i++){
		d = mesh.vertex_list[i].distance(point);		
		if (d<dmin && d<= maxRadius){
			dmin = d;
			closest = mesh.vertex_list[i];
			min_vertex_idx = i;
			vlist_in_sphere.insert(i);
		}	
	}
	cout << "Closest Vertex to Query Point ";
	cout << closest.x <<" "<< closest.y <<" "<< closest.z;
	cout << ", Distance " << dmin<< endl;
	

	/*if (dmin == std::numeric_limits<float>::infinity())
		return NULL;*/
	return std::pair<float,Vec3f>(dmin,closest);
	//return closest;

}

/*Find distance of point on face, of only those faces which have
vertices or edges within the maximum search radius from queryPoint*/

std::vector<Face> ClosestPoint::fetchFacesinSphere(){
	/*cout << "Valid Vertex sizes "<< vlist_in_sphere.size()<<endl;
	cout << " Valid Edges sizes "<< edge_in_sphere.size()<<endl;*/

	std::set <int> valid_verts;
	set_union(vlist_in_sphere.begin(),vlist_in_sphere.end(),
						edge_in_sphere.begin(), edge_in_sphere.end(),
                  		std::inserter(valid_verts,valid_verts.begin()));
	//cout << "Union size "<<valid_verts.size()<<endl;
	std::vector<Face> result;
	
	for (int i =0; i< mesh.face_list.size(); i++){	
		/*Create set of current face vertices*/	
		std::set<int> temp;
		for (int j =0; j<3; j++ ){
			temp.insert(mesh.face_list[i].vid[j]);
		}
		
		/*for (set<int>::iterator j = temp.begin(); j != temp.end(); j++) {
   			int element = *j;
   			cout << element<< " "<< endl;
   		}*/
   		/*Check interesction with Bounding Sphere*/
   		set<int> intersect;
		set_intersection(temp.begin(),temp.end(),valid_verts.begin(),
						valid_verts.end(),
                  		std::inserter(intersect,intersect.begin()));
   		if (intersect.size()>0)
   			result.push_back(mesh.face_list[i]);   		
	}	
	return result;

}

int main(int argc, char** argv){
	Mesh mesh;
	
	//char* filename = "./bunny.ply";
	char* filename = "data/happy.ply";

	if (!mesh.readPly(filename)){
		cout <<"Failed to read ply file"<<endl;
		return 1;
	}
	ClosestPoint cp = ClosestPoint(mesh);
	cp.getEdgeList();

	cp(Vec3f(-0.0083935,0.065325,-0.0472749),0.01);
	cp(Vec3f(-0.0083935,0.23,-0.0472749),0.10);
	cp(	Vec3f(10.0,10.0,12.0),10.0);
	//cp.fetchFacesinSphere();
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
