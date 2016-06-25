#ifndef SCENE_H
#define SCENE_H

/* custom_classes.h:
 *
 * these are ad-hoc structures used by this specific game:
 *  - Stats, (of a ship)
 *  - Ship,
 *  - Bullet,
 *  - Scene (all data of a game, also the arena)
 *
 * (in class, this file was called "scene.h")
 *
 * Often, they inhert from general "PhysObjects" and add fields and/or
 * add / redefine funcionalies. They use methods of their base class.
 *
 */
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
	void render() const;

	void setMaxVelAndAcc( float maxVel, float acc );
	std::vector< Bullet > bullets;
	float timeBeforeFiringAgain;
	void spawnNewBullet();

	Bullet& findUnusedBullet();
	void fillBullet(Bullet& b) const;

	void reset();
	void die();

	void setStatsAsFighter(); //
	void setStatsAsTank();
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
	void renderFloor() const;
	void cameraOnTwoObjects( const PhysObject& a, const PhysObject& b );
};

extern Scene scene; // a poor man's singleton (there is one, and everyone can use it)

#endif // SCENE_H
