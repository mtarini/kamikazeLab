#ifndef AIMIND_H
#define AIMIND_H

#include "controller.h"
#include "scene.h"

class AiMind {
public:
	const Ship* me = NULL;
	const Ship* target = NULL;

	void rethink( ShipController& output );

	AiMind():me(NULL),target(NULL) { me = target = NULL;}

};



#endif // AIMIND_H
