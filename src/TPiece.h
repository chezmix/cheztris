//TPiece.h - The T-shaped tetris piece

//  X X X 0
//  0 X 0 0
//	0 0 0 0
//  0 0 0 0


#ifndef __TPIECE_H__
#define __TPIECE_H__

#include "TetrisPiece.h"



class TPiece : public TetrisPiece {
public:

	TPiece();
	~TPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	TPiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__TPIECE_H__