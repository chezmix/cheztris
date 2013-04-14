//TetrisBoardMultiManager.h - Mode with multiple TetrisBoards in play

#ifndef __TETRISBOARDMULTIMANAGER_H__
#define __TETRISBOARDMULTIMANAGER_H__

#include "TetrisBoardManager.h"
#include <vector>
#include <queue>

class TetrisBoard;
class Gui;

class TetrisBoardMultiManager : public TetrisBoardManager {
public:
	TetrisBoardMultiManager(Gui * g);
	~TetrisBoardMultiManager();

	void add_tetrisboard();
	void remove_tetrisboard(TetrisBoard * t);
	void cycle();
	bool handle_input();
	void draw();
	void draw_hud();
	void draw_controls();
	void draw_game_over();
	void switch_hold_piece();
	void select_previous_tetrisboard();
	void select_next_tetrisboard();
	void select_tetrisboard(int id);

	//Reporter Functions
	void report_piece_drop(TetrisBoard * T);
	void report_line_clear(TetrisBoard * T, int count);

private:
	bool allBoardsLocked();
	void unlockBoards();

	Gui * gui;
	std::vector<TetrisBoard *> tetrisboards;
	std::vector<bool> board_locks;
	std::queue<TetrisBoard *> remove_queue;
	
	int active_id;
	int num_boards_to_add;
	int multiplier;
	int timer;
	int timer_duration;
	int chain_count;
	
	TetrisPiece * hold_piece;
	bool hold_used;
	int level;
	int lines;
	int score;
	int last_keyhold_time;
	int last_cycle_time;

};

#endif //__TETRISBOARDMULTIMANAGER_H__