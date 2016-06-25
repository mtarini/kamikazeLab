
#include "scene.h"

Scene scene;

void Ship::setMaxVelAndAcc(float maxSpeed, float acc){
	stats.accRate = acc;

	// compute drag so that limit speed is maxSpeed
	drag = acc / maxSpeed;
}

float randInZeroToOne(){
	return ((rand()%1001))/1000.0f;
}

float randInMinusOneToOne(){
	return ((rand()%2001)-1000)/1000.0f;
}


vec3 randomFlatUnitVec(){
	return vec3(
				randInMinusOneToOne(),
				randInMinusOneToOne(),
				0
				);
}

void Ship::reset(){
	timeBeforeFiringAgain = 0.0; // ready!
	PhysObject::reset();
	for (Bullet &b : bullets) b.reset();

	t.pos = scene.randomPosInArena();
}

void Ship::die(){
	t.pos = scene.randomPosInArena();
}

vec3 Scene::randomPosInArena() const{
	return randomFlatUnitVec() *arenaRadius;
}


Bullet& Ship::findUnusedBullet(){
	for (Bullet &b : bullets) {
		if (!b.alive) return b;
	}
	return bullets[0]; // this should never happen
}

void Ship::spawnNewBullet(){
	Bullet &b = findUnusedBullet();
	fillBullet( b );
}

void Ship::fillBullet(Bullet &b) const {
	b.alive = true;
	b.t.pos = t.pos; // TODO: put where the gun hole is (in Space shape)

	b.timeToLive = stats.fireRange / stats.fireSpeed;
	b.vel = t.forward() * stats.fireSpeed ;
	b.angVel = quat(1,0,0,0);

	b.mass = 0.1f;
	b.coll.radius = 0.03f;
	// TODO: maybe randomize a bit pos and vel

}

bool Scene::isInside( vec3 p ) const{
	return ( p.x>=-arenaRadius && p.x<=arenaRadius &&
			 p.y>=-arenaRadius && p.y<=arenaRadius );
}

vec3 Scene::pacmanWarp( vec3 p) const{
	vec3 res = p;
	if (res.x>+arenaRadius) res.x -= arenaRadius*2;
	if (res.y>+arenaRadius) res.y -= arenaRadius*2;
	if (res.x<-arenaRadius) res.x += arenaRadius*2;
	if (res.y<-arenaRadius) res.y += arenaRadius*2;
	return res;
}

void Scene::initAsNewGame(){
	arenaRadius = 60;
	ships.resize(2);

	for (Ship &s: ships) {
		s.bullets.resize( 100 ); // move in init
		s.reset();

		s.coll.radius = 0.8f;
		s.mass = 10.0; // KG!

	}
	ships[0].setStatsAsFighter();
	ships[1].setStatsAsTank();

}

void Ship::setStatsAsFighter(){
	stats.turnRate = 1040; // deg / s^2
	setMaxVelAndAcc( 30.0f, 60.0f ); // m/s, m/s^2
	stats.fireRate = 8;  // shots per sec
	stats.fireRange = 8.0; // m
	stats.fireSpeed = 30.0; // m/s
}

void Ship::setStatsAsTank(){
	stats.turnRate = 800; // deg / s^2
	setMaxVelAndAcc( 50.0f, 10.0f ); // m/s, m/s^2
	stats.fireRate = 2.0;  // shots per sec
	stats.fireRange = 50.0; // m
	stats.fireSpeed = 20.0; // m/s
}

