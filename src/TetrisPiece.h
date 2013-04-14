//TetrisPiece.h - Abstract class for one of the six Tetris Pieces in the game

#ifndef __TETRISPIECE_H__
#define __TETRISPIECE_H__

#define BLACK 0
#define RED 1
#define GREEN 2 
#define BLUE 3
#define YELLOW 4
#define ORANGE 5
#define PURPLE 6
#define TEAL 7
#define GREY 8

static const int box_state1[16] = {BLUE, BLUE, 0, 0, BLUE, BLUE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const int long_state1[16] = {0, 0, 0, 0, GREEN, GREEN, GREEN, GREEN, 0, 0, 0, 0, 0, 0, 0, 0};
static const int long_state2[16] = {0, 0, GREEN, 0, 0, 0, GREEN, 0, 0, 0, GREEN, 0, 0, 0, GREEN, 0};

static const int j_state1[16] = {YELLOW, 0, 0, 0, YELLOW, YELLOW, YELLOW, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int j_state2[16] = {YELLOW, YELLOW, 0, 0, YELLOW, 0, 0, 0, YELLOW, 0, 0, 0, 0, 0, 0, 0};
static const int j_state3[16] = {YELLOW, YELLOW, YELLOW, 0, 0, 0, YELLOW, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int j_state4[16] = {0, YELLOW, 0, 0, 0, YELLOW, 0, 0, YELLOW, YELLOW, 0, 0, 0, 0, 0, 0};

static const int l_state1[16] = {0, 0, ORANGE, 0, ORANGE, ORANGE, ORANGE, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int l_state2[16] = {ORANGE, 0, 0, 0, ORANGE, 0, 0, 0, ORANGE, ORANGE, 0, 0, 0, 0, 0, 0};
static const int l_state3[16] = {ORANGE, ORANGE, ORANGE, 0, ORANGE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int l_state4[16] = {ORANGE, ORANGE, 0, 0, 0, ORANGE, 0, 0, 0, ORANGE, 0, 0, 0, 0, 0, 0};

static const int s_state1[16] = {0, RED, RED, 0, RED, RED, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int s_state2[16] = {RED, 0, 0, 0, RED, RED, 0, 0, 0, RED, 0, 0, 0, 0, 0, 0};

static const int two_state1[16] = {TEAL, TEAL, 0, 0, 0, TEAL, TEAL, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int two_state2[16] = {0, TEAL, 0, 0, TEAL, TEAL, 0, 0, TEAL, 0, 0, 0, 0, 0, 0, 0};

static const int t_state1[16] = {0, PURPLE, 0, 0, PURPLE, PURPLE, PURPLE, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int t_state2[16] = {0, PURPLE, 0, 0, 0, PURPLE, PURPLE, 0, 0, PURPLE, 0, 0, 0, 0, 0, 0};
static const int t_state3[16] = {0, 0, 0, 0, PURPLE, PURPLE, PURPLE, 0, 0, PURPLE, 0, 0, 0, 0, 0, 0};
static const int t_state4[16] = {0, PURPLE, 0, 0, PURPLE, PURPLE, 0, 0, 0, PURPLE, 0, 0, 0, 0, 0, 0};

static const int square_state1[16] = {GREY, GREY, GREY, 0, GREY, 0, GREY, 0, GREY, GREY, GREY, 0, 0, 0, 0, 0 };

static const int super_square_state1[16] = {GREY, GREY, GREY, GREY, GREY, 0, 0, GREY, GREY, 0, 0, GREY, GREY, GREY, GREY, GREY };


class TetrisPiece {
public:
	virtual ~TetrisPiece() {};
	virtual void rotate() = 0;
	virtual void rotate_back() = 0;
	virtual int * get_state() = 0;
	virtual TetrisPiece * clone() = 0;

protected:
	int state[16];
	int active_state;
};



#endif //__TETRISPIECE_H__