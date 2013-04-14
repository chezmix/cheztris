//BoxPiece.h - The Box shaped tetris piece

//  X X 0 0
//  X X 0 0
//	0 0 0 0
//  0 0 0 0


#ifndef __BOXPIECE_H__
#define __BOXPIECE_H__

#include "TetrisPiece.h"



class BoxPiece : public TetrisPiece {
public:

	BoxPiece();
	~BoxPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	BoxPiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__BOXPIECE_H__