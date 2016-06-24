#ifndef CONTROLLER_H
#define CONTROLLER_H


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
