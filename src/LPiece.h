//LPiece.h - The L shaped tetris piece

//  X 0 0 0
//  X 0 0 0
//	X X 0 0
//  0 0 0 0


#ifndef __LPIECE_H__
#define __LPIECE_H__

#include "TetrisPiece.h"



class LPiece : public TetrisPiece {
public:

	LPiece();
	~LPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	LPiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__LPIECE_H__