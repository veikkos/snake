#include "ai.h"

Ai::Ai() {

}

Ai::~Ai() {

}

void Ai::GetDir(Snake *ai_snake, Eatable *cur_eatable) {

	Direction old_dir = ai_snake->GetDir(), new_dir;

	new_dir = old_dir;

	// Get direction based on the eatable...
	if((ai_snake->GetDir() == s_right) || (ai_snake->GetDir() == s_left)) {

		if(ai_snake->GetPosY(0) < cur_eatable->GetPosY(0)) {
			new_dir = s_down;
		}
		else if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0) ) {
			new_dir = s_up;
		}

	} else if((ai_snake->GetDir() == s_up) || (ai_snake->GetDir() == s_down)) {

		if(ai_snake->GetPosX(0) < cur_eatable->GetPosX(0)) {
			new_dir = s_right;
		}
		else if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)) {
			new_dir = s_left;
		}
	}

	// ...and check that snake doesn't crash to itself on next block.
	if(CollisionIn(ai_snake, new_dir) == false) {
		ai_snake->SetDir(new_dir);
		return;
	}

	// Prioritize the first two options with the distance to
	// the eatable.
	switch(new_dir) {

		case s_right: {
			if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0)) {
				if(CollisionIn(ai_snake, s_up) == false) {
					ai_snake->SetDir(s_up);
					return;
				} else if(CollisionIn(ai_snake, s_down) == false) {
					ai_snake->SetDir(s_down);
					return;
				}
			} else {
				if(CollisionIn(ai_snake, s_down) == false) {
					ai_snake->SetDir(s_down);
					return;
				} else if(CollisionIn(ai_snake, s_up) == false) {
					ai_snake->SetDir(s_up);
					return;
				}
			}

			if(CollisionIn(ai_snake, s_left) == false) {
				ai_snake->SetDir(s_left);
				return;
			}

			break;
		}

		case s_left: {
			if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0)) {
				if(CollisionIn(ai_snake, s_up) == false) {
					ai_snake->SetDir(s_up);
					return;
				} else if(CollisionIn(ai_snake, s_down) == false) {
					ai_snake->SetDir(s_down);
					return;
				}
			} else {
				if(CollisionIn(ai_snake, s_down) == false) {
					ai_snake->SetDir(s_down);
					return;
				} else if(CollisionIn(ai_snake, s_up) == false) {
					ai_snake->SetDir(s_up);
					return;
				}
			}

			if(CollisionIn(ai_snake, s_right) == false) {
				ai_snake->SetDir(s_right);
				return;
			}

			break;
		}

		case s_down: {
			if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)) {
				if(CollisionIn(ai_snake, s_left) == false) {
					ai_snake->SetDir(s_left);
					return;
				} else if(CollisionIn(ai_snake, s_right) == false) {
					ai_snake->SetDir(s_right);
					return;
				}
			} else {
				if(CollisionIn(ai_snake, s_right) == false) {
					ai_snake->SetDir(s_right);
					return;
				} else if(CollisionIn(ai_snake, s_left) == false) {
					ai_snake->SetDir(s_left);
					return;
				}
			}

			if(CollisionIn(ai_snake, s_up) == false) {
				ai_snake->SetDir(s_up);
				return;
			}

			break;
		}

		case s_up: {
			if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)) {
				if(CollisionIn(ai_snake, s_left) == false) {
					ai_snake->SetDir(s_left);
					return;
				} else if(CollisionIn(ai_snake, s_right) == false) {
					ai_snake->SetDir(s_right);
					return;
				}
			} else {
				if(CollisionIn(ai_snake, s_right) == false) {
					ai_snake->SetDir(s_right);
					return;
				} else if(CollisionIn(ai_snake, s_left) == false) {
					ai_snake->SetDir(s_left);
					return;
				}
			}

			if(CollisionIn(ai_snake, s_down) == false) {
				ai_snake->SetDir(s_down);
				return;
			}

			break;
		}
	}
}

bool Ai::CollisionIn(Snake *ai_snake, Direction dir) {

	int i;

	for(i=1; i<ai_snake->GetLength(); i++) {

		switch(dir) {

			case s_right: {
				if((ai_snake->GetPosX(0) + GRID_SIZE) == ai_snake->GetPosX(i)) {

					if(ai_snake->GetPosY(0) == ai_snake->GetPosY(i)) {
						return true;
					}
				}

				break;
			}

			case s_left: {
				if((ai_snake->GetPosX(0) - GRID_SIZE) == ai_snake->GetPosX(i)) {

					if(ai_snake->GetPosY(0) == ai_snake->GetPosY(i)) {
						return true;
					}
				}

				break;
			}

			case s_down: {
				if((ai_snake->GetPosY(0) + GRID_SIZE) == ai_snake->GetPosY(i)) {

					if(ai_snake->GetPosX(0) == ai_snake->GetPosX(i)) {
						return true;
					}
				}

				break;
			}

			case s_up: {
				if((ai_snake->GetPosY(0) - GRID_SIZE) == ai_snake->GetPosY(i)) {

					if(ai_snake->GetPosX(0) == ai_snake->GetPosX(i)) {
						return true;
					}
				}

				break;
			}
		}
	}

	return false;
}
