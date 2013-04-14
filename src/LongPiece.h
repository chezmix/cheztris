//LongPiece.h - The Long shaped tetris piece

//  X 0 0 0
//  X 0 0 0
//	X 0 0 0
//  X 0 0 0


#ifndef __LONGPIECE_H__
#define __LONGPIECE_H__

#include "TetrisPiece.h"



class LongPiece : public TetrisPiece {
public:

	LongPiece();
	~LongPiece();
	void rotate();
	void rotate_back();
	int * get_state();
	LongPiece * clone();

private:
	void assign_state(int statenum, const int * s);

};


#endif //__LONGPIECE_H__