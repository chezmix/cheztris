#include "JPiece.h"

/* Default Position
   XX00
   0X00
   0X00
   0000 */

JPiece::JPiece() {
	assign_state(1, j_state1);
}
JPiece::~JPiece() {}

void JPiece::rotate() {
	if (active_state == 1) {
		assign_state(2, j_state2);
	} else if (active_state == 2) {
		assign_state(3, j_state3);
	} else if (active_state == 3) {
		assign_state(4, j_state4);
	} else {
		assign_state(1, j_state1);
	}
}

void JPiece::rotate_back() {
	if (active_state == 1) {
		assign_state(4, j_state4);
	} else if (active_state == 2) {
		assign_state(1, j_state1);
	} else if (active_state == 3) {
		assign_state(2, j_state2);
	} else {
		assign_state(3, j_state3);
	}
}

int * JPiece::get_state() {
	return state;
}

void JPiece::assign_state(int statenum, const int * s) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[i];
	}
	active_state = statenum;
}

JPiece * JPiece::clone() {
	JPiece * clone = new JPiece();
	clone->assign_state(active_state, state);
	return clone;
}