#ifndef AIMIND_H
#define AIMIND_H

#include "controller.h"
#include "scene.h"

class AiMind {
public:
	const Ship* me = NULL;
	const Ship* target = NULL;

	float alertness = 0.75f; // 0 = sleeping  1 = terminator
	float happyTrigger = 1.3f; // if bullet will fly within this dist from target: FIRE!

	void rethink( ShipController& output );

	AiMind(){ }

};



#endif // AIMIND_H
