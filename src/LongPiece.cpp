#include "LongPiece.h"

/* Default Position
   X000
   X000
   X000
   X000 */

LongPiece::LongPiece() {
	assign_state(1, long_state1);
}

LongPiece::~LongPiece() {}

void LongPiece::rotate() {
	if (active_state == 1) {
		assign_state(2, long_state2);
	} else {
		assign_state(1, long_state1);
	}
}

void LongPiece::rotate_back() {
	rotate();
}

int * LongPiece::get_state() {
	return state;
}

void LongPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

LongPiece * LongPiece::clone() {
	LongPiece * clone = new LongPiece();
	clone->assign_state(active_state, state);
	return clone;
}