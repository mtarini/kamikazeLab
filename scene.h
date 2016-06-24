#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "phys_object.h"
#include "controller.h"

struct Stats{
	float accRate;
	float turnRate;
	float fireRate; // bullets per seconds
	float fireRange;
	float fireSpeed;
};

struct Bullet : public PhysObject {
	float timeToLive;
	bool alive;
	void renderPlaceHolder() const;
	void doPhysStep();
	void reset(){ alive = false; }
};


struct Ship: public PhysObject{

	Stats stats;

	ShipController controller;

	void doPhysStep();

	void renderPlaceHolder() const;

	void setMaxVelAndAcc( float maxVel, float acc );

	std::vector< Bullet > bullets;

	float timeBeforeFiringAgain;

	void spawnNewBullet();


	Bullet& findUnusedBullet();
	void fillBullet(Bullet& b) const;

	void reset();
	void die();\
};

struct Scene{

	float arenaRadius;
	std::vector< Ship > ships;

	vec3 randomPosInArena() const;
	void initAsNewGame();
	void render();
	void doPhysStep();

	bool isInside( vec3 p ) const;
	vec3 pacmanWarp( vec3 p) const;

private:
	void checkAllCollisions();

};

extern Scene scene; // a poor man's singleton

#endif // SCENE_H
