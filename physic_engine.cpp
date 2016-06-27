/* physic_engine.cpp :
 *
 * all methods of all classes, and all global functions, which deal with physics:
 * dynamics (here, with Euler), collisions detection, etc
 */

#include <math.h>
#include "custom_classes.h"

const float dt = 1.0f/30; // in secs

void PhysObject::doPhysStep(){

	/*TODO: consider forces . e.g. graviy
	vec3 acc = force / mass;
	vel += acc * dt;*/

	t.pos += vel * dt;
	t.ori *= glm::slerp( quat(1,0,0,0) , angVel ,  dt*10 );

	// damping of angular velocity
	angVel = glm::slerp( quat(1,0,0,0) , angVel , 1.0f - angDrag*dt );
	// damping of linear velocity
	vel *= 1.0-drag*dt; // an approximation : // (1-D)^dt = (1-D*dt)

	//vel *= pow(1.0-drag,double(dt));

	/*
	const float rpm = 60; // rot-per-minute

	float turnAngle = pi<float>()*2;
	t.ori *= angleAxis( rpm * dt/60.0f *turnAngle , vec3(0,0,1) );
	*/
}

bool collides(const PhysObject &a ,
			  const PhysObject &b )
{
	/*if (a.coll.type == Collider::SPHERE &&
		b.coll.type == Collider::SPHERE) */
	{
		// test sphere VS sphere:
		return dot(
					a.t.pos - b.t.pos ,
					a.t.pos - b.t.pos
			   )
			   <
			   (a.coll.radius + b.coll.radius)*
			   (a.coll.radius + b.coll.radius);
	}

}

void enforceSeparate(PhysObject &a, PhysObject &b){
	float currDist = length(a.t.pos - b.t.pos);
	float minDist = a.coll.radius + b.coll.radius;
	if (currDist>minDist) return;

	float diff = minDist - currDist; // positive!

	vec3 dir = (a.t.pos - b.t.pos)/currDist;
	a.t.pos += dir * diff*( b.mass / ( a.mass + b.mass ) );
	b.t.pos -= dir * diff*( a.mass / ( a.mass + b.mass ) );

}

void Bullet::doPhysStep(){
	if (!alive) return;
	timeToLive -= dt;
	if (timeToLive<=0) alive = false;

	t.pos += vel*dt;
}

void Ship::doPhysStep(){

	if (alive) {
		/* PARTE VOLONTARIA: */
		if (controller.status[ ShipController::LEFT ]){
			angVel *= glm::angleAxis( glm::radians(+stats.turnRate)*dt,vec3(0,0,1));
		}
		if (controller.status[ ShipController::RIGHT ]){
			angVel *= glm::angleAxis( glm::radians(-stats.turnRate)*dt,vec3(0,0,1));
		}
		if (controller.status[ ShipController::GO ]){
			vel += t.forward() * (stats.accRate * dt);
		}

		if (timeBeforeFiringAgain<=0) {
			if (controller.status[ ShipController::FIRE ]) {
				spawnNewBullet();
				timeBeforeFiringAgain += 1 / stats.fireRate ;
			}
		} else timeBeforeFiringAgain -= dt;

		// graphics: make it do a roll according to angular velocity
		float rollAngle = glm::angle(angVel) *
				sign(dot(glm::axis(angVel),vec3(0,0,1)))
				* 1.3f
				* (length(vel)*0.055f+1.0f);
		meshComponent.t.ori =
				glm::angleAxis( rollAngle, vec3(0,-1,0) ) *
				quat( -sqrt(2.0f)/2.0f,0,0,sqrt(2.0f)/2 )  ;
	}
	else {
		timeDead+=dt;
		if (timeDead>1.0) respawn();
	}

	/* PARTE PASSIVA */
	PhysObject::doPhysStep();

	if (!scene.isInside( t.pos )) vel *= -0.8;
	//t.pos = scene.pacmanWarp( t.pos );

	/* BULLETS */
	for (Bullet& b: bullets) b.doPhysStep();
}

void Scene::checkAllCollisions(){
	if (collides(ships[0],ships[1])) {
		// collision response: ship VS ship
		std::swap( ships[0].vel, ships[1].vel );
		enforceSeparate(ships[0],ships[1]);
	}

	for (Bullet &b : ships[0].bullets) if (b.alive){
		if ( collides(b,ships[1]) ) ships[1].die();
	}

	for (Bullet &b : ships[1].bullets) if (b.alive) {
		if ( collides(b,ships[0]) ) ships[0].die();
	}
}

void Scene::doPhysStep(){
	for (Ship &s : ships){
		s.doPhysStep(); // also move bullets
	}
	checkAllCollisions();
}

