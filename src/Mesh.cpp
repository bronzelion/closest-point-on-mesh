#include <Mesh.h>
#include <iostream>
#include <exception>
Mesh::Mesh(const Mesh &obj){
	vertex_list = obj.vertex_list;
	face_list = obj.face_list;
	nverts = obj.nverts;
	nfaces = obj.nfaces;
}
/*int** Mesh::getEdgeList(){

//int size = vertex_list.size();
int** edgeList = (int**)calloc(nverts, sizeof((int*)calloc(nverts, sizeof(int))));
cout <<"Im here"<<face_list[0].nverts;

//Loop through all faces, prepare sparse adjacency matrix
	for (int i =0 ; i< face_list.size(); i++){
		cout << "Begin"<<face_list.size();
  		int n = face_list[i].nverts;
  		cout << "N" << n;
  		for(int i = 0; i< n;  i++){
			int idx1 = i;
			int idx2 = (i+1) % n;

			if (edgeList[idx1][idx2] != 1)
			edgeList[idx1][idx2] = 1;
  		}
	}
	return edgeList;
}

*/

bool Mesh::readPly(char *filename){

typedef struct face_struct {

  unsigned char nverts;
  int *verts;
  float nx,ny,nz;
} face_struct;

int vertices;
PlyFile *plyFile;
PlyElement **elems;
PlyProperty *property,**ele_prop;
FILE *fp;

PlyProperty vert_props[] = {
  {"x", Float32, Float32, offsetof(Vec3f,x), 0, 0, 0, 0},
  {"y", Float32, Float32, offsetof(Vec3f,y), 0, 0, 0, 0},
  {"z", Float32, Float32, offsetof(Vec3f,z), 0, 0, 0, 0},
};

PlyProperty face_props[] = {
  {"vertex_indices",Uint8 , Int32, offsetof(face_struct,verts), 1, Uint8, Uint8,offsetof(face_struct,nverts)}
};

float arr;
int numOfElements,nprops,i,k,j;
char *ele_name;
char **elements;

  fp = fopen(filename,"r");
  plyFile = read_ply(fp);
  if (fp == NULL){
  	cout<< "Error reading ply file "<< filename<<endl;
  	return false;
  }

try{
  for(i=0; i< plyFile->num_elem_types; i++){
	 elems = plyFile->elems;
	 ele_name = elems[i]->name;
	 numOfElements=elems[i]->num;

	 cout<< "Number of "<< ele_name<< ": "<<elems[i]->num<<endl;
	 nprops = elems[i]->nprops;
	 ele_prop = elems[i]->props;
	 //cout<<ele_prop[0]->name;

  if(equal_strings ("vertex", ele_name)){
	vertices=elems[i]->num;        
	//vlist = (Vertex **) malloc (sizeof (Vertex *) * elems[i]->num);

	ply_get_property (plyFile, ele_name, &vert_props[0]);
	ply_get_property (plyFile, ele_name, &vert_props[1]);
	ply_get_property (plyFile, ele_name, &vert_props[2]);               

	float* vlist = (float*) calloc (3,sizeof (float));
	Vec3f temp;
	for (j = 0; j < numOfElements; j++) {
	  
	  ply_get_element (plyFile, (void *) vlist);
		
		temp.x = vlist[0];
		temp.y = vlist[1];
		temp.z = vlist[2];

	  vertex_list.push_back(temp);
	  //cout << vertex_list[j].x << vertex_list[j].y << vertex_list[j].z<<endl;
	   }
	 }

  if (equal_strings ("face", ele_name)) {	  
	ply_get_property (plyFile, ele_name, &face_props[0]);
	//cout << "face_props" << face_props[0];
	 //= (Face*) malloc (sizeof (Face));
	face_struct *flist = (face_struct*) malloc (sizeof (face_struct));
	Face face_obj;
	int faces = elems[i]->num;
	for (j = 0; j < elems[i]->num; j++) {
	  //flist[j] = (Face *) malloc (sizeof (Face));
	  ply_get_element (plyFile,(void*)flist);

	  //cout << "face" << (float)flist->verts[0]<<endl;
		 face_obj.nverts = (float)flist->nverts;
		 face_obj.vid = flist->verts;
		 face_obj.v1 = vertex_list[(int)flist->verts[0]];
		 face_obj.v2 = vertex_list[(int)flist->verts[1]];
		 face_obj.v3 = vertex_list[(int)flist->verts[2]];
		 face_obj.id = j;
		 face_list.push_back(face_obj);
		 //cout << "id" <<face_obj.vid[0]<<face_obj.vid[2]<<endl;
	 }
  }
  }
  nverts = vertex_list.size();
  nfaces = face_list.size();
 }
 catch(exception& e){
 	cout << "Hit exception during ply parse, only ASCII formats supported "<<e.what();
 }
  return true;
}


