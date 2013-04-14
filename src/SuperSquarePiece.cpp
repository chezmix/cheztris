#include "SuperSquarePiece.h"

/* Default Position
   XXXX
   X00X
   X00X
   XXXX */

SuperSquarePiece::SuperSquarePiece() {
	assign_state(1, super_square_state1);
}

SuperSquarePiece::~SuperSquarePiece() {}

void SuperSquarePiece::rotate() {

}

void SuperSquarePiece::rotate_back() {
	
}

int * SuperSquarePiece::get_state() {
	return state;
}

void SuperSquarePiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

SuperSquarePiece * SuperSquarePiece::clone() {
	SuperSquarePiece * clone = new SuperSquarePiece();
	clone->assign_state(active_state, state);
	return clone;
}