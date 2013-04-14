#include "TwoPiece.h"

/* Default Position
   XX00
   0XX0
   0000
   0000 */

TwoPiece::TwoPiece() {
	assign_state(1, two_state1);
}

TwoPiece::~TwoPiece() {}

void TwoPiece::rotate() {
	if (active_state == 1) {
		assign_state(2, two_state2);
	} else {
		assign_state(1, two_state1);
	}
}

void TwoPiece::rotate_back() {
	rotate();
}

int * TwoPiece::get_state() {
	return state;
}

void TwoPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

TwoPiece * TwoPiece::clone() {
	TwoPiece * clone = new TwoPiece();
	clone->assign_state(active_state, state);
	return clone;
}