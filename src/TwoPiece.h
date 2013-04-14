//TwoPiece.h - The Two shaped tetris piece

//  X X 0 0
//  0 X X 0
//	0 0 0 0
//  0 0 0 0


#ifndef __TWOPIECE_H__
#define __TWOPIECE_H__

#include "TetrisPiece.h"



class TwoPiece : public TetrisPiece {
public:

	TwoPiece();
	~TwoPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	TwoPiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__TWOPIECE_H__