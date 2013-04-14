//SPiece.h - The S shaped tetris piece

//  0 X X 0
//  X X 0 0
//	0 0 0 0
//  0 0 0 0


#ifndef __SPIECE_H__
#define __SPIECE_H__

#include "TetrisPiece.h"



class SPiece : public TetrisPiece {
public:

	SPiece();
	~SPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	SPiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__SPIECE_H__