/* asset_manager.cpp :
 * all methods of all classes and all global functions which deal with managing assets:
 * Importing, procedural construction, maybe a bit of pre-processing
 */

#include <fstream>
#include <sstream>
#include <string>

#include"mesh.h"
#include"custom_classes.h"
#include"texture.h"

void preloadAllAssets(){

	// quick hack: change me!
	std::string assetPath = "C:/corsi/game_engines_2016/kamikazeLab2016/assets/";

	CpuMesh tmp;
	tmp.import(assetPath + "dark_fighter_6.obj");
	//tmp.buildTorus(10,30,5.0,15.0); // or, procedural creation

	CpuTexture tmpText;
	tmpText.import(assetPath + "dark_fighter_6_color.pbm");
	//tmpText.createRandom(256);
	GpuTexture gpuText = tmpText.uploadToGPU();

	MeshComponent comp;
	comp.mesh = tmp.uploadToGPU();
	comp.texture = gpuText;
	// let set a tranform manually to adapt the asset to our needs
	comp.t.setIde();
	comp.t.scale = 0.05f;
	comp.t.ori = quat( -sqrt(2.0f)/2.0f,0,0,sqrt(2.0f)/2.0f );

	// TODO: differntiate models / textures
	scene.ships[0].meshComponent = comp;
	scene.ships[1].meshComponent = comp;

}

void CpuMesh::addQuad(int i, int j, int k, int h){
	tris.push_back( Tri(i,j,k) );
	tris.push_back( Tri(k,h,i) );
}

void CpuMesh::buildTorus(int ni, int nj, float innerRadius, float outerRadius){

	verts.reserve( ni*nj );
	tris.reserve( ni*nj*2 );

	for (int j=0; j<nj; j++)
	for (int i=0; i<ni; i++) {

		// construct geometry
		double alpha = 2*3.1415 * i / ni;
		float x = (float)cos(alpha) * innerRadius;
		float y = (float)sin(alpha) * innerRadius;
		x += outerRadius;

		double beta = 2*3.1415 * j / nj;

		Vertex v;
		v.pos.x = (float)cos(beta) * x;
		v.pos.y = y;
		v.pos.z = (float)sin(beta) * x;

		verts.push_back( v );

		// construct connectivity
		addQuad( j        * ni +  i       ,
				 j        * ni + (i+1)%ni ,
				 (j+1)%nj * ni + (i+1)%ni ,
				 (j+1)%nj * ni +  i       );
	}

}

bool CpuMesh::import(const std::string& filename){
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
			tmpV.push_back( vec3(x,z,y) );
		} else if (code=="vn") {
			float x,y,z;
			iss >> x >> y >> z;
			tmpN.push_back( vec3(x,z,y) );
		} else if (code=="vt") {
			float x,y;
			iss >> x >> y;
			Vertex nv;
			nv.uv = vec2(x,1.0f-y); // NB: flipping the y, different conventions about UV space
			verts.push_back( nv );
			// v.pos e v.norm: ci penso dopo, quando leggero' le facce
		} else if (code=="f") {
			std::string st_i, st_j, st_k;
			iss >> st_i >> st_j >> st_k;
			int i0,i1,i2;
			int j0,j1,j2;
			int k0,k1,k2;
			sscanf( st_i.c_str() , "%d/%d/%d" , &i0,&i1,&i2 );
			sscanf( st_j.c_str() , "%d/%d/%d" , &j0,&j1,&j2 );
			sscanf( st_k.c_str() , "%d/%d/%d" , &k0,&k1,&k2 );

			//  Obj indices start from 1 not 0!
			i0--;j0--;k0--;
			i1--;j1--;k1--;
			i2--;j2--;k2--;

			Tri nt( i1, j1, k1 );
			tris.push_back( nt );

			verts[ i1 ].pos = tmpV[ i0 ];
			verts[ i1 ].norm = tmpN[ i2 ];
			verts[ j1 ].pos = tmpV[ j0 ];
			verts[ j1 ].norm = tmpN[ j2 ];
			verts[ k1 ].pos = tmpV[ k0 ];
			verts[ k1 ].norm = tmpN[ k2 ];

		}

	}
	return true;

}

bool CpuTexture::import(std::string filename){
	std::ifstream infile(filename,std::ios::binary);
	if (!infile.is_open()) return false;

	int depth;
	std::string token;
	infile >> token >> sizeX >> sizeY >> depth;
	// TODO: some checking please!

	data.reserve( sizeX*sizeY );

	for (int i=0; i<sizeX*sizeY; i++) {
		char rgb[3];
		infile.read(rgb, 3 );
		Texel t;
		t.r = rgb[0];
		t.g = rgb[1];
		t.b = rgb[2];
		data.push_back( t );
	}

	return true;
}

void CpuTexture::createRandom(int size){
	sizeX = sizeY = size;
	data.resize(sizeX * sizeY);
	for (Texel &t : data) {
		t.r = rand()%256;
		t.g = rand()%256;
		t.b = rand()%256;
		t.a = 255;
	}
}
