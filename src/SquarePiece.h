//SquarePiece.h - The Square shaped tetris piece (Used in Versus mode only)

//  X X X 0
//  X 0 X 0
//	X X X 0
//  0 0 0 0


#ifndef __SQUAREPIECE_H__
#define __SQUAREPIECE_H__

#include "TetrisPiece.h"



class SquarePiece : public TetrisPiece {
public:

	SquarePiece();
	~SquarePiece();
	void rotate();
	void rotate_back();
	int * get_state();
	SquarePiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__SQUAREPIECE_H__