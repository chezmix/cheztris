#include "SPiece.h"


/* Default Position
   0XX0
   XX00
   0000
   0000 */

SPiece::SPiece() {
	assign_state(1, s_state1);
}
SPiece::~SPiece() {}

void SPiece::rotate() {
	if (active_state == 1) {
		assign_state(2, s_state2);
	} else {
		assign_state(1, s_state1);
	}
}

void SPiece::rotate_back() {
	rotate();
}

int * SPiece::get_state() {
	return state;
}

void SPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

SPiece * SPiece::clone() {
	SPiece * clone = new SPiece();
	clone->assign_state(active_state, state);
	return clone;
}