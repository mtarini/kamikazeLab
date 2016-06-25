/* ai.cpp
 * everything about the NPC behaviour (AI).
 */

#include "aimind.h"

float randInZeroToOne(); // small helper function, defined somewhere

// time at which the two objects a and b will be at their closest
float minDistTime( const PhysObject& a, const PhysObject& b ){
	float t = -dot( a.t.pos-b.t.pos , a.vel-b.vel ) /
				dot( a.vel-b.vel, a.vel-b.vel );
	if (t<0) return 0; // the minimial distance time was it THE PAST!
	else return t;
}

bool willItCollide( const PhysObject& a, const Bullet& b , float tolerance,
					bool &turnRight , bool &tooFar ){
	float dt = minDistTime( a , b );
	vec3 aFuture = a.t.pos + a.vel * dt;
	vec3 bFuture = b.t.pos + b.vel * dt;

	vec3 minDist = aFuture - bFuture;

	vec3 rotAxis( 0,0,1 );
	float tripleProduct = dot( cross( b.vel, rotAxis ) , minDist );

	float sumOfRadii = a.coll.radius + b.coll.radius + tolerance;

	turnRight = (tripleProduct > 0);
	tooFar = (dt > b.timeToLive);

	return ( dot( minDist, minDist) < sumOfRadii * sumOfRadii );
}

void AiMind::rethink(ShipController &output){

	if (!me) return;

	if (randInZeroToOne() > alertness) return;

	Bullet hypoteticalBullet; // se sparassi ORA
	me->fillBullet( hypoteticalBullet );

	bool goR , goF;
	bool doFire = willItCollide( *target , hypoteticalBullet , happyTrigger, goR , goF );

	output.status[ ShipController::FIRE ] = doFire && !goF ;
	output.status[ ShipController::LEFT ] = (!goR);
	output.status[ ShipController::RIGHT ] = (goR);
	output.status[ ShipController::GO ] = goF ;
}
