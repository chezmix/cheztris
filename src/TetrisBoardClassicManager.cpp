#include "TetrisBoardClassicManager.h"
#include "TetrisBoard.h"
#include "Gui.h"
#include <sstream> //for converting ints to strings

TetrisBoardClassicManager::TetrisBoardClassicManager(Gui * g) : gui(g), hold_piece(NULL), hold_used(false), lines(0), level(1), score(0), last_keyhold_time(0)
{
	the_board = new TetrisBoard(this, screen_width/2, screen_height/2 - screen_height/8, 10, 20);
}

TetrisBoardClassicManager::~TetrisBoardClassicManager()
{
	delete the_board;
}

void TetrisBoardClassicManager::cycle() {
	gui->cycle();	
	the_board->cycle();
	if (the_board->is_dead()) {
		game_over = true;
	}
}

bool TetrisBoardClassicManager::handle_input() {
/* The Controls 
 * UP - Rotate Piece
 * LEFT - Move Piece Left
 * RIGHT - Move Piece Right
 * DOWN - Move Piece Down
 * LEFT SHIFT - Switch/Store Hold Piece
*/

	Uint8 *keys;
	SDL_Event event;
	const int KEY_HOLD_DELAY = 160; //Time it takes to re-register a held down key press (in milliseconds)
	keys = SDL_GetKeyState(NULL);
	bool done = false;

	//Handle Single Stroke Input 
	while ( SDL_PollEvent(&event) ) {
		switch(event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_SPACE:
						the_board->sink_active();
						break;
					case SDLK_w:
					case SDLK_UP:
						the_board->rotate_active();
						break;
					case SDLK_a:
					case SDLK_LEFT:
						the_board->move_active_left();
						last_keyhold_time = SDL_GetTicks();
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						the_board->move_active_right();
						last_keyhold_time = SDL_GetTicks();
						break;
					case SDLK_s:
					case SDLK_DOWN:
						the_board->move_active_down();
						last_keyhold_time = SDL_GetTicks();
						break;
					case SDLK_LCTRL:
					case SDLK_RCTRL:
						switch_hold_piece();
						break;
				}
				break;
			case SDL_QUIT:
				done = true;
				break;
		}
	} // end input


	//Handle Continuous Input
	if (SDL_GetTicks() - last_keyhold_time > KEY_HOLD_DELAY) {
		if ( keys[SDLK_LEFT] || keys[SDLK_a] )  { 
			the_board->move_active_left();
			last_keyhold_time = SDL_GetTicks();
		}
		if ( keys[SDLK_RIGHT] || keys[SDLK_d] ) { 
			the_board->move_active_right();
			last_keyhold_time = SDL_GetTicks();
		}
		if ( keys[SDLK_DOWN] || keys[SDLK_s] ) { 
			the_board->move_active_down();
			last_keyhold_time = SDL_GetTicks();
		}
	}
	return done;
}

void TetrisBoardClassicManager::draw() {
	int square_size = screen_height/30;
	the_board->draw(square_size,1.0,1.0,1.0);
	draw_hold_box(hold_piece, hold_used, square_size, screen_width/2 - 100, screen_height - 135);
}

void TetrisBoardClassicManager::draw_hud() {
	gui->draw_string("Hold", 310, 555, 255, 255, 255);
	gui->draw_string("Next", 435, 555, 255, 255, 255);

	gui->draw_string("Level ", 25, 150, 255, 128, 0);
	gui->draw_int(level, 50, 125, 255, 255, 255);
	gui->draw_string("Lines ", 25, 100, 255, 128, 0);
	gui->draw_int(lines, 50, 75, 255, 255, 255);
	gui->draw_string("Score ", 25, 50, 255, 128, 0);
	gui->draw_int(score, 50, 25, 255, 255, 255);
}

void TetrisBoardClassicManager::draw_controls() {
	gui->draw_string("Classic", 375, 550, 0, 255, 0);

	gui->draw_string("Controls", 50, 500, 255, 128, 0);

	gui->draw_string("Rotate", 75, 450, 255, 255, 255);
	gui->draw_string("Move Left", 75, 420, 255, 255, 255);
	gui->draw_string("Move Down", 75, 390, 255, 255, 255);
	gui->draw_string("Move Right", 75, 360, 255, 255, 255);
	gui->draw_string("Drop", 75, 330, 255, 255, 255);
	gui->draw_string("Hold", 75, 300, 255, 255, 255);
	gui->draw_string("Quit", 75, 270, 255, 255, 255);

	gui->draw_string("Up or W", 250, 450, 255, 255, 255);
	gui->draw_string("Left or A", 250, 420, 255, 255, 255);
	gui->draw_string("Down or S", 250, 390, 255, 255, 255);
	gui->draw_string("Right or D", 250, 360, 255, 255, 255);
	gui->draw_string("Space Bar", 250, 330, 255, 255, 255);
	gui->draw_string("Control", 250, 300, 255, 255, 255);
	gui->draw_string("Escape", 250, 270, 255, 255, 255);

	gui->draw_string("Hints", 50, 200, 255, 128, 0);
	gui->draw_string("+ Make lines with the blocks to clear them from the board.", 75, 150, 255, 255 ,255);
	gui->draw_string("+ Clearing multiple lines at once will will give you bonus points.", 75, 120, 255, 255 ,255);
	gui->draw_string("+ Every 10 lines the blocks will fall faster!", 75, 90, 255, 255 ,255);

	gui->draw_string("Press Enter to Start!", 600, 10, 255, 255 ,255);
}

void TetrisBoardClassicManager::draw_game_over() {
	gui->draw_string("GAME OVER!", 50, 200, 255, 255, 255);
	gui->draw_string("Your score: ", 50, 280, 255, 255, 255);
	gui->draw_int(score, 170, 280, 255, 255, 255);
	gui->draw_string("Press Enter to continue...", 540, 10, 255, 255, 255);
}

void TetrisBoardClassicManager::switch_hold_piece() {
	if (!hold_used) {
		TetrisPiece * temp = hold_piece;
		hold_piece = the_board->get_active_piece();
		the_board->set_active_piece(temp);
		hold_used = true;
	}
}


//Reporters

void TetrisBoardClassicManager::report_piece_drop(TetrisBoard * T) {
	score += 10;
	hold_used = false;
}
void TetrisBoardClassicManager::report_line_clear(TetrisBoard * T, int num_lines) {
	std::stringstream ss;
	std::string string_type, string_score, string_multiplier;

	int clear_score;

	switch (num_lines){
		case 1: 
			string_type = "Single!";
			break;
		case 2:
			string_type = "Double";
			break;
		case 3:
			string_type = "Triple";
			break;
		case 4:
			string_type = "Chez-Tris!?";
			break;
	}

	clear_score = (num_lines * num_lines * 100);
	ss << clear_score;
	ss >> string_score;

	score += clear_score;
	lines += num_lines;
	level = lines / 10 + 1;
	if (level > 10) {
		level = 10;
	}
	the_board->set_speed(10*level);

	gui->add_text_animation(string_type, the_board->get_xPos()-25, the_board->get_yPos()+25, 100);
	gui->add_text_animation(" +" + string_score, the_board->get_xPos()-25, the_board->get_yPos(), 100);
}