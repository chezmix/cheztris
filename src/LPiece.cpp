#include "LPiece.h"

/* Default Position
   X000
   X000
   XX00
   0000 */

LPiece::LPiece() {
	assign_state(1, l_state1);
}

LPiece::~LPiece() {}

void LPiece::rotate() {
	if (active_state == 1) {
		assign_state(2, l_state2);
	} else if (active_state == 2) {
		assign_state(3, l_state3);
	} else if (active_state == 3) {
		assign_state(4, l_state4);
	} else {
		assign_state(1, l_state1);
	}
}

void LPiece::rotate_back() {
	if (active_state == 1) {
		assign_state(4, l_state4);
	} else if (active_state == 2) {
		assign_state(1, l_state1);
	} else if (active_state == 3) {
		assign_state(2, l_state2);
	} else {
		assign_state(3, l_state3);
	}
}

int * LPiece::get_state() {
	return state;
}

void LPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

LPiece * LPiece::clone() {
	LPiece * clone = new LPiece();
	clone->assign_state(active_state, state);
	return clone;
}
