#ifndef MESH_H
#define MESH_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
using namespace glm;

#include "transform.h"

struct Vertex{
	vec3 pos;
	vec3 norm;
	vec2 uv;
};

struct Tri{
	int i,j,k;
	Tri(int _i, int _j, int _k):i(_i),j(_j),k(_k){}
};

/* (ids of) a mesh structure in GPU ram */
struct GpuMesh{
	void render() const;
	uint geomBufferId = 666; // "name" of GPU buffer for vertices
	uint connBufferId = 666; // "name" of GPU buffer for triangles
	int nElements = 0;
};

/* a mesh structure in CPU ram */
struct CpuMesh{
	std::vector< Vertex > verts;
	std::vector< Tri > tris;

	bool import(const std::string& filename);
	void renderDeprecated(); // uses immediate mode

	// todo: a bit of geometry processing...
	void updateNormals();
	void resize( float scale );
	void flipYZ();
	void apply(Transform t);

	// procedural constructions..
	void buildTorus(int ni, int nj, float innerRadius, float outerRadius);

	GpuMesh uploadToGPU();

private:

	void addQuad(int i, int j, int k, int h);

};

struct MeshComponent{
	Transform t;
	GpuMesh mesh;
	// textures, materials...
};

#endif // MESH_H
