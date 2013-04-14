//TetrisBoardSurvivalManager.h - Mode where player tries to "survive" the longest

#ifndef __TETRISBOARDSURVIVALMANAGER_H__
#define __TETRISBOARDSURVIVALMANAGER_H__

#define ATTACK_SPEED 1
#define ATTACK_TIMER 2
#define ATTACK_SHRINK 3

#include "TetrisBoardManager.h"

class TetrisBoard;
class Gui;

class TetrisBoardSurvivalManager : public TetrisBoardManager {
public:
	TetrisBoardSurvivalManager(Gui * g);
	~TetrisBoardSurvivalManager();

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
	int lines;
	int speed;
	int last_keyhold_time;
	int attack_delay;			//Default time between attacks (in milliseconds)
	int start_time;				//Time of game start (in milliseconds)
	int elapsed_timer;			//Elapsed time since game started (in milliseconds)
	int attack_start_time;		//Time of most recent attack timer start (in milliseconds)
	int attack_timer;			//Time before the next attack (in milliseconds)
	int bonus_time;				//Accumulated bonus time to be added to attack timer (in milliseconds)
	int save_threshold;			//maximum value of attack_timer time to award save bonus (in milliseconds)

	void attack();
};

#endif //__TETRISBOARDSURVIVALMANAGER_H__