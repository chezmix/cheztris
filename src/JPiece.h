//JPiece.h - The J shaped tetris piece

//  0 X 0 0
//  0 X 0 0
//	X X 0 0
//  0 0 0 0


#ifndef __JPIECE_H__
#define __JPIECE_H__

#include "TetrisPiece.h"



class JPiece : public TetrisPiece {
public:

	JPiece();
	~JPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	JPiece * clone();

private:
	void assign_state(int statenum, const int * s);
};


#endif //__JPIECE_H__