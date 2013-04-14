//SuperSquarePiece.h - The Giant Square shaped tetris piece (Used in Versus mode only)

//  X X X X
//  X 0 0 X
//	X 0 0 X
//  X X X X


#ifndef __SUPERSQUAREPIECE_H__
#define __SUPERSQUAREPIECE_H__

#include "TetrisPiece.h"



class SuperSquarePiece : public TetrisPiece {
public:

	SuperSquarePiece();
	~SuperSquarePiece();
	void rotate();
	void rotate_back();
	int * get_state();
	SuperSquarePiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__SUPERSQUAREPIECE_H__