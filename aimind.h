#ifndef AIMIND_H
#define AIMIND_H

/* AiMind: a class for modelling the mind of the behaviour
 *
 * current state: no high or medium level goals:
 * just one: seek and kill "target"
 *
 * TODO:
 *   add fear (new medium level goal: elude dangers)
 *   add selection of medium level goals (e.g. attack or evade?)
 *   add selection of target of said goal (e.g. whom to attack or evade)
 *   etc.
 */
#include "controller.h"
#include "custom_classes.h"

class AiMind {
public:
	const Ship* me = NULL;
	const Ship* target = NULL;

	float alertness = 0.35f; // 0 = sleeping  1 = terminator
	float happyTrigger = 1.5f; // if bullet will fly within this dist from target: FIRE!

	void rethink( ShipController& output );

	void setTerminator(){
		alertness = 1.0;
		happyTrigger = 0.0; // only shoot if you really think you'll hit
	}

	void setHumanLike(){
		alertness = 0.35f; // only react 35% of frames
		happyTrigger = 1.5; // happy to shoot even if not the perfect shot
	}

	AiMind(){ }

};



#endif // AIMIND_H
