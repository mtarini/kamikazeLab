#ifndef CONTROLLER_H
#define CONTROLLER_H

/* Controller:
 *
 * the immediate "will" of a ship.
 * 1- Directly wired to keyboard for human players,
 * 2- The result of AI decision making procedure,
 * 3- In a LAN, with deterministic lockstep: what is communicated between clients
 * 4- For e.g. a replay: what is saved for each player per-frame to record an "action"
 *
 * (3 and 4: TODO)
 *
 */

struct ShipController{
	enum {LEFT, RIGHT, GO, FIRE , N_STATUS};
	bool status[ N_STATUS ];

	void reset(){
		for (int i=0; i<N_STATUS; i++) status[i]=false;
	}

	int key[ N_STATUS ]; // which key controls this catus

	ShipController(){
		reset();
	}

	void soakKey( int newKey , bool newStatus){
		for (int i=0; i<N_STATUS; i++) {
			if (key[i]==newKey) status[i]=newStatus;
		}
	}

	void useWASD();
	void useArrows();
};

#endif // CONTROLLER_H
