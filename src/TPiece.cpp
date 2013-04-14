#include "TPiece.h"

/* Default Position
   XXX0
   0X00
   0000
   0000 */

TPiece::TPiece() {
	assign_state(1, t_state1);
}

TPiece::~TPiece() {}

void TPiece::rotate() {
	if (active_state == 1) {
		assign_state(2, t_state2);
	} else if (active_state == 2) {
		assign_state(3, t_state3);
	} else if (active_state == 3) {
		assign_state(4, t_state4);
	} else {
		assign_state(1, t_state1);
	}
}

void TPiece::rotate_back() {
	if (active_state == 1) {
		assign_state(4, t_state4);
	} else if (active_state == 2) {
		assign_state(1, t_state1);
	} else if (active_state == 3) {
		assign_state(2, t_state2);
	} else {
		assign_state(3, t_state3);
	}
}

int * TPiece::get_state() {
	return state;
}

void TPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

TPiece * TPiece::clone() {
	TPiece * clone = new TPiece();
	clone->assign_state(active_state, state);
	return clone;
}