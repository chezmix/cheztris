//TetrisBoardManager.h - Manages all tetrisboards in play

#ifndef __TETRISBOARDMANAGER_H__
#define __TETRISBOARDMANAGER_H__

class TetrisBoard;
class TetrisPiece;

extern int screen_width;
extern int screen_height;

class TetrisBoardManager {
public:
	TetrisBoardManager();
	virtual ~TetrisBoardManager() {};
	virtual void cycle() = 0;
	virtual bool handle_input() = 0;
	virtual void draw() = 0;
	virtual void draw_hud() = 0;
	virtual void draw_controls() = 0;
	virtual void draw_game_over() = 0;

	//Reporter Functions
	virtual void report_piece_drop(TetrisBoard * T) = 0;
	virtual void report_line_clear(TetrisBoard * T, int count) = 0;

	bool is_game_over() const;

protected:
	void draw_hold_box(TetrisPiece * hold_piece, bool hold_used, int square_size, int x, int y);

	bool game_over;
};

#endif //__TETRISBOARDMANAGER_H__