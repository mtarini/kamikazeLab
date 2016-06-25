#include <fstream>
#include <sstream>
#include <string>

#include"mesh.h"

void Mesh::addQuad(int i, int j, int k, int h){
	tris.push_back( Tri(i,j,k) );
	tris.push_back( Tri(k,h,i) );
}

void Mesh::buildTorus(int ni, int nj, float innerRadius, float outerRadius){

	verts.reserve( ni*nj );
	tris.reserve( ni*nj*2 );

	for (int j=0; j<nj; j++)
	for (int i=0; i<ni; i++) {

		// construct geometry
		double alpha = 2*3.1415 * i / ni;
		float x = cos(alpha) * innerRadius;
		float y = sin(alpha) * innerRadius;
		x += outerRadius;

		double beta = 2*3.1415 * j / nj;

		Vertex v;
		v.pos.x = cos(beta) * x;
		v.pos.y = y;
		v.pos.z = sin(beta) * x;

		verts.push_back( v );

		// construct connectivity
		addQuad( j        * ni +  i       ,
				 j        * ni + (i+1)%ni ,
				 (j+1)%nj * ni + (i+1)%ni ,
				 (j+1)%nj * ni +  i       );
	}

}

bool Mesh::import(const std::string& filename){
	std::ifstream infile(filename);
	if (!infile.is_open()) return false;
	std::string line;

	std::vector< vec3 > tmpV;
	std::vector< vec3 > tmpN;

	while ( std::getline(infile, line) ){

		std::istringstream iss(line);

		std::string code;
		iss >> code;
		if (code=="v") {
			float x,y,z;
			iss >> x >> y >> z;
			tmpV.push_back( vec3(x,y,z) );
		} else if (code=="vn") {
			float x,y,z;
			iss >> x >> y >> z;
			tmpN.push_back( vec3(x,y,z) );
		} else if (code=="vt") {
			float x,y;
			iss >> x >> y;
			Vertex v;
			v.uv = vec2(x,y);
			// v.pos e v.norm ci penso dopo
		} else if (code=="f") {
			int i,j,k;
			std::string st_i, st_j, st_k;
			iss >> st_i >> st_j >> st_k;
			int i0,i1,i2;
			int j0,j1,j2;
			int k0,k1,k2;
			sscanf( st_i.c_str() , "%d/%d/%d" , &i0,&i1,&i2 );
			sscanf( st_j.c_str() , "%d/%d/%d" , &j0,&j1,&j2 );
			sscanf( st_k.c_str() , "%d/%d/%d" , &k0,&k1,&k2 );

		}

	}
	return true;

}

