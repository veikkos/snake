#include "eatables.h"
#include "defines.h"
#include "entity.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

Eatable::Eatable(eatable_type n_type,
                 vector<position> *used = NULL) : Entity(1) {
	bool block_used;

	do {

		block_used = false;

		pos[0]->x = nearest(rand() % (X_AREA - GRID_SIZE), GRID_SIZE);
		pos[0]->y = nearest(rand() % (Y_AREA - GRID_SIZE), GRID_SIZE);

		if(used) {
			for (std::vector<position>::iterator iter = used->begin();
			        iter != used->end(); ++iter) {
				if(pos[0]->x == (*iter).x && pos[0]->y == (*iter).y) {
					block_used = true;
					break;
				}
			}
		}

	} while(block_used);

	if(n_type == e_dynamic) {
		frame = E_DYN_FRAMES - 1;
	}
	else {
		frame = 0;
	}

	alive = 1;
	tick = 0;
	type = n_type;
}

Eatable::~Eatable() {

}

bool Eatable::Update() {

	if(tick++ > E_DYN_FRAME_AGE) {

		tick = 0;

		if(frame > 0) {
			frame--;
		} else {
			alive = 0;
			return false;
		}
	}

	return true;
}

int Eatable::GetFrame() {

	return frame;
}

int Eatable::nearest(int value, int near) {

	int rest = value % near;

	if( rest > near/2 ) {
		return (value - rest) + near;
	}
	else {
		return (value-rest);
	}
}
