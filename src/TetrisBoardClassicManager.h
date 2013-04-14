//TetrisBoardClassManager.h - Mode that closly resembles classic Tetris

#ifndef __TETRISBOARDCLASSICMANAGER_H__
#define __TETRISBOARDCLASSICMANAGER_H__

#include "TetrisBoardManager.h"

class TetrisBoard;
class Gui;

class TetrisBoardClassicManager : public TetrisBoardManager {
public:
	TetrisBoardClassicManager(Gui * g);
	~TetrisBoardClassicManager();

	void cycle();
	bool handle_input();
	void draw();
	void draw_hud();
	void draw_controls();
	void draw_game_over();
	void switch_hold_piece();

	//Reporter Functions
	void report_piece_drop(TetrisBoard * T);
	void report_line_clear(TetrisBoard * T, int count);

private:
	Gui * gui;
	TetrisBoard * the_board;
	TetrisPiece * hold_piece;
	bool hold_used;
	int level;
	int lines;
	int score;
	int last_keyhold_time;

};

#endif //__TETRISBOARDCLASSICMANAGER_H__