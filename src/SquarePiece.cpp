#include "SquarePiece.h"

/* Default Position
   XXXX
   X0X0
   XXX0
   0000 */

SquarePiece::SquarePiece() {
	assign_state(1, square_state1);
}

SquarePiece::~SquarePiece() {}

void SquarePiece::rotate() {

}

void SquarePiece::rotate_back() {
	
}

int * SquarePiece::get_state() {
	return state;
}

void SquarePiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

SquarePiece * SquarePiece::clone() {
	SquarePiece * clone = new SquarePiece();
	clone->assign_state(active_state, state);
	return clone;
}