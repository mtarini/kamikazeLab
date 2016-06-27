#ifndef PHYS_OBJECT_H
#define PHYS_OBJECT_H

#include "transform.h"
#include "collider.h"
#include "mesh.h"

/* class PhysObject:
 *  a physical entithy if our virtual world.
 *  It has
 *    - a transform ("where it is"),
 *    - physical properties, like mass, speed, angular speed...
 *    - more components, including:
 *      * a physical estension (a Collider)
 *      * a way it looks (a MeshComponent)
 *
 * TODO: make a hierarchical structure, i.e. make it a node of a SceneGraph.
 */

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
	float angDrag;

	void reset(){
		vel = vec3(0,0,0);
		angVel = quat(1,0,0,0);
		t.setIde();
	}

	void setCameraInside();
};

bool collides(const PhysObject &a ,
			  const PhysObject &b );

void enforceSeparate(PhysObject &a , PhysObject &b );


#endif // PHYS_OBJECT_H
