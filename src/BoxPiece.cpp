#include "BoxPiece.h"

/* Default Position
   XX00
   XX00
   0000
   0000 */

BoxPiece::BoxPiece() {
	assign_state(1, box_state1);
}

BoxPiece::~BoxPiece() {}

void BoxPiece::rotate() {

}

void BoxPiece::rotate_back() {
	
}

int * BoxPiece::get_state() {
	return state;
}

void BoxPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

BoxPiece * BoxPiece::clone() {
	BoxPiece * clone = new BoxPiece();
	clone->assign_state(active_state, state);
	return clone;
}