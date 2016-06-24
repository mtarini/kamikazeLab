#ifndef PHYS_OBJECT_H
#define PHYS_OBJECT_H

#include "transform.h"
#include "collider.h"

class PhysObject{
public:
	Transform t;
	Collider coll;

	vec3 vel;
	float mass;

	void renderPlaceHolder() const;
	void doPhysStep();

	float drag;

	void reset(){
		vel = vec3(0,0,0);
		t.setIde();
		drag = 0.03f;
	}
};

bool collides(const PhysObject &a ,
			  const PhysObject &b );

void enforceSeparate(PhysObject &a , PhysObject &b );


#endif // PHYS_OBJECT_H
