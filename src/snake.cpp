#include "snake.h"
#include "defines.h"
#include <string.h>

Snake::Snake(int x, int y, int len) : Entity(MAX_LENGTH) {
	int i;

	// Set head position

	pos[0]->x = x;
	pos[0]->y = y;

	for(i=1; i<len; i++) {
		pos[i]->x = x - GRID_SIZE * i;
		pos[i]->y = y;
	}

	dir = s_right;
	length = len;
	speed = 1;
	pause = 0;
}

Snake::~Snake() {

}

void Snake::SetDir(Direction n_dir) {

	if(dir == s_left && n_dir == s_right) {
		return;
	}
	else if(dir == s_right && n_dir == s_left) {
		return;
	}
	else if(dir == s_up && n_dir == s_down) {
		return;
	}
	else if(dir == s_down && n_dir == s_up) {
		return;
	}

	dir = n_dir;
}

int Snake::GetLength() {

	return length;
}

Direction Snake::GetDir() {

	return dir;
}

void Snake::Grow(int bits) {

	int i, o_length;

	o_length = length;

	if((length + bits) > MAX_LENGTH) {
		length = MAX_LENGTH;
		bits = (MAX_LENGTH - o_length);
	} else {
		length += bits;
	}

	for(i=0; i<bits; i++) {
		*pos[o_length + i] = *pos[o_length - 1];
	}
}

void Snake::Move() {

	int i;

	if(pause++ < SNAKE_PAUSE) {
		return;
	}
	else {
		pause = 0;
	}

	for(i=length-1; i>0; i--) {
		*pos[i] = *pos[i-1];
	}

	switch(dir) {

		case s_right:

			if((pos[0]->x + GRID_SIZE * speed) < X_AREA) {
				pos[0]->x += GRID_SIZE * speed;
			}
			else {
				pos[0]->x = 0;
			}

			break;

		case s_down:

			if((pos[0]->y + GRID_SIZE * speed) < Y_AREA) {
				pos[0]->y += GRID_SIZE * speed;
			}
			else {
				pos[0]->y = 0;
			}

			break;

		case s_left:

			if((pos[0]->x - GRID_SIZE * speed) >= 0) {
				pos[0]->x -= GRID_SIZE * speed;
			}
			else {
				pos[0]->x = X_AREA - GRID_SIZE;
			}

			break;

		case s_up:

			if((pos[0]->y - GRID_SIZE * speed) >= 0) {
				pos[0]->y -= GRID_SIZE * speed;
			}
			else {
				pos[0]->y = Y_AREA - GRID_SIZE;
			}

			break;
	}
}
