#include "ClosestPoint.h"
#include <fstream>
#include <cmath>
#include <boost/test/minimal.hpp>

#define INFINITY_FLOAT std::numeric_limits<float>::infinity();

/*Test case to validate if the point is in bounds*/
bool pointInSphere(Vec3f result, Vec3f queryPoint, float radius){
 	if (result == NULL)
 		return true;
 	return (pow(result.x-queryPoint.x,2) + 
 			pow(result.y-queryPoint.y,2) + 
 			pow(result.z-queryPoint.z,2) <= pow(radius,2));
}

/*Helper function*/
bool barycentric (Vec3f a, Vec3f b, Vec3f c, Vec3f p){
	Vec3f v0 = c -a ;
	Vec3f v1 = b -a ;
	Vec3f v2 = p -a ;

	float dot00 = v0.dot(v0);
	float dot01 = v0.dot(v1);
	float dot02 = v0.dot(v2);
	float dot11 = v1.dot(v1);
	float dot12 = v1.dot(v2);

	double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v <= 1);
}

/*Test case to check if the point actually exists on the mesh*/
bool pointOnMesh(Vec3f output,Mesh m){
	if (output == NULL)
 		return true;
	vector<Face> face = m.get_faceList();
	for (int i=0; i<face.size() ; i++){
		Face temp = face[i];
		
		if(barycentric(temp.v1,temp.v2,temp.v3,output))
			return true;
	}
 	return false;
}

/*Boost Test Main*/
int test_main(int argc, char** argv){
	Mesh mesh;
	char* filename = "./data/bunny.ply";	 //Default input mesh

	if(argc ==2){
		std::string argv1 = argv[1];		
		if((argv1 == "-h" )|| (argv1 == "--help")){
			cout <<"Usage: ./ClosestPoint [path to ply file]"<<endl;
			cout << "Default ply: ./data/bunny.ply"<<endl;
			exit(0);
		}
		else
			filename = argv[1];		
	}

	cout << "Reading file: "<<filename<<endl;	

	/*Populate Mesh Data structure from the Ply file*/
	
	if (!mesh.readPly(filename)){
		cout <<"Failed to read ply file "<< filename<<endl;
		return 1;
	}

	ClosestPoint cp = ClosestPoint(mesh);    

	/*Read query points from a file and run the algorithm!*/
	std::ifstream infile("./data/queryPoints.txt");
	float x,y,z,searchRadius;
	
	while(infile >>x >> y >> z >> searchRadius){		
		Vec3f queryPoint = Vec3f(x,y,z);
		Vec3f output = cp(queryPoint,searchRadius);		

		BOOST_CHECK(pointInSphere(output,queryPoint,searchRadius) == true);
		BOOST_CHECK(pointOnMesh(output,cp.getMesh()) == true);
	}	
	return 0;
}