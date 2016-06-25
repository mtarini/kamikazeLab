#ifndef PHYS_OBJECT_H
#define PHYS_OBJECT_H

#include "transform.h"
#include "collider.h"
#include "mesh.h"

class PhysObject{
public:
	Transform t;

	// components
	Collider coll;
	MeshComponent meshComponent;

	vec3 vel;
	quat angVel;

	float mass;

	void render() const;
	void doPhysStep();

	float drag;

	void reset(){
		vel = vec3(0,0,0);
		angVel = quat(1,0,0,0);
		t.setIde();
		drag = 0.03f;
	}
};

bool collides(const PhysObject &a ,
			  const PhysObject &b );

void enforceSeparate(PhysObject &a , PhysObject &b );


#endif // PHYS_OBJECT_H
