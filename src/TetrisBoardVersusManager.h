//TetrisBoardClassManager.h - Mode that closly resembles classic Tetris

#ifndef __TETRISBOARDVERSUSMANAGER_H__
#define __TETRISBOARDVERSUSMANAGER_H__

#define NOITEM 0
#define SPEED 1
#define STUN 2
#define SQUARE 3
#define SUPERSPEED 4
#define SUPERSTUN 5
#define SUPERSQUARE 6

#include "TetrisBoardManager.h"

class TetrisBoard;
class Gui;

class TetrisBoardVersusManager : public TetrisBoardManager {
public:
	TetrisBoardVersusManager(Gui * g);
	~TetrisBoardVersusManager();

	void cycle();
	bool handle_input();
	void draw();
	void draw_hud();
	void draw_controls();
	void draw_game_over();
	void p1_switch_hold_piece();
	void p2_switch_hold_piece();

	//Reporter Functions
	void report_piece_drop(TetrisBoard * T);
	void report_line_clear(TetrisBoard * T, int count);

private:
	Gui * gui;
	TetrisBoard * p1;
	TetrisBoard * p2;
	TetrisPiece * p1_hold_piece;
	TetrisPiece * p2_hold_piece;
	bool p1_hold_used;
	bool p2_hold_used;
	int p1_meter;
	int p2_meter;
	int p1_last_keyhold_time;
	int p2_last_keyhold_time;
	int p1_item;
	int p2_item;
	int p1_effect;			 //Type of effect board is suffering
	int p2_effect;
	int p1_effect_duration;  //Measured in # piece drops left
	int p2_effect_duration;
	int num_items_used;
	bool is_p1_board(TetrisBoard * T);
	void draw_meters();
	void p1_activate_item();
	void p2_activate_item();
};

#endif //__TETRISBOARDVERSUSMANAGER_H__