//TetrisBoard.h - A grid (normal =  10x20) representing the Tetris playing field
//Notes: Top of the board is line # 0, bottom of the board is line # <board_height-1>
//		 For a WxH grid there are (W+2)x(H+2) grid entries
//		 Example for 3x6 grid:  (3+2)x(6+2) = 40 grid entries in the array
//
//	Line # 0:		|0  1  2 | 3  4
//	Line # 1:		|5  6  7 | 8  9
//	Line # 2:		|10 11 12| 13 14
//	Line # 3:		|15 16 17| 18 19
//	Line # 4:		|20 21 22| 23 24
//	Line # 5:		|25 26 27| 28 29
//					|--------|------
//					|30 31 32| 33 34
//					|35 36 37| 38 39
//
//		Elements within | | represent the playable region.  Grid elements outside the playable region exist so that pieces 
//		rotated on the right edge of the playable region have room to rotate properly.

#ifndef __TETRISBOARD_H__
#define __TETRISBOARD_H__

#include <windows.h>
#include <GL/gl.h>
#include "TetrisPiece.h"

//#include "Gui.h"

class TetrisBoardManager;

class TetrisBoard {
public:
	TetrisBoard(TetrisBoardManager * tbm, int x, int y, int width, int height);
	TetrisBoard(const TetrisBoard &t); //Copy Constructor
	~TetrisBoard();

	void draw(int square_size, float r, float g, float b); //draw tetrisboard with center (x,y)
	void cycle();  // Execute one movement cycle
	void rotate_active();
	void move_active_left();
	void move_active_right();
	void move_active_down();
	void sink_active();
	void decrease_board_height();

	//Animations
	void add_move_animation(int dx);  //move board dx units

	//Accessors
	bool is_dead();
	int get_xPos();
	int get_yPos();
	TetrisPiece * get_active_piece();

	//Setters
	void set_board_width(int w);
	void set_xPos(int x);
	void set_yPos(int y);
	//void set_board_offset_x(int x);
	//void set_board_offset_y(int y);
	//void set_square_size(int size);
	void set_speed(int spd);
	void set_cycles(int c);
	void set_active_piece(TetrisPiece* t);  //carfully manage the pointer to the old piece to prevent memory leak
	void set_next_piece(TetrisPiece* t);
	

private:
	void init_grid();
	void draw_block(int x, int y, int square_size, int color, bool shadow);
	void draw_shadow_piece(int square_size);
	bool right_collision();
	bool left_collision();
	bool bottom_collision();
	bool block_collision();
	void update_grid();
	void remove_active_from_grid();
	void remove_lines();
	void resolve_bottom_collision();
	void shift_grid_down(int line);
	void shift_grid_up();
	void set_block_color(int num, bool shadow);
	TetrisPiece * generate_piece();

	int * grid; 
	TetrisPiece * active_piece;
	TetrisPiece * next_piece;
	//TetrisPiece * hold_piece;
	int active_piece_x;
	int active_piece_y;
	int xPos;  //x Position on screen
	int yPos;	//y Position on screen
	//bool hold_exists;
	//bool hold_used;
	bool dead;
	//int score;
	//int level;
	//int lines;
	int speed; //0-100  100 = fastest
	int board_width;
	int board_height;
	//int board_offset_x;
	//int board_offset_y;
	int square_size;
	//Gui * gui;
	TetrisBoardManager * TBM;
	int last_cycle_time;
	int cycles;
	bool move_animation_active;
	int move_animation_delta_x;
};



#endif //__TETRISBOARD_H__