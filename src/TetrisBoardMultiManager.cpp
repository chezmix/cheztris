#include "TetrisBoardMultiManager.h"
#include "TetrisBoard.h"
#include "Gui.h"
#include <vector>
#include <sstream> //for converting ints to strings

TetrisBoardMultiManager::TetrisBoardMultiManager(Gui * g) : gui(g), active_id(0), num_boards_to_add(0), multiplier(1), timer(0), chain_count(0), hold_piece(NULL), hold_used(false), lines(0), level(1), score(0), last_keyhold_time(0), last_cycle_time(0), timer_duration(3000)
{
	add_tetrisboard();
	//add_tetrisboard();
	//add_tetrisboard();
	//add_tetrisboard();
}

TetrisBoardMultiManager::~TetrisBoardMultiManager()
{
	std::vector<TetrisBoard *>::iterator itr;

	for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
		delete *itr;
	}

	delete hold_piece;
}

//Cycle all tetrisboards
void TetrisBoardMultiManager::cycle() {
	std::vector<TetrisBoard *>::iterator itr;

	gui->cycle();	
	int i = 0;
	int speed;
	for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
		(*itr)->cycle();
		if (i == active_id) {
			speed = 10 + 10*(lines/150);
			if (speed > 100) {
				speed = 100;
			}
			speed -= 8 * (tetrisboards.size() - 1);
		} else {
			if (board_locks.at(i)) {
				speed = 0;
			} else {
				speed = 8;
			}
		}
		(*itr)->set_speed(speed);
		if ((*itr)->is_dead()) {
			remove_queue.push(*itr);
			unlockBoards();
			multiplier = tetrisboards.size() - 1;
			timer = 0;
		}
		i++;
	}

	//Remove any pending tetrisboards after all boards have cycled
	while (remove_queue.size() > 0) {
		remove_tetrisboard(remove_queue.front());
		remove_queue.pop();
	}

	//Add any pending tetrisboards after all boards have cycled
	for (int i = 0; i < num_boards_to_add; i++) {
		add_tetrisboard();
	}
	num_boards_to_add = 0;
	if (timer > 0) {
		timer -= (SDL_GetTicks() - last_cycle_time);
	} else {
		timer = 0;
		multiplier = tetrisboards.size();
		chain_count = 0;
		unlockBoards();
	}
	last_cycle_time = SDL_GetTicks();
}

bool TetrisBoardMultiManager::handle_input() {
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
	TetrisBoard * active_tetrisboard = tetrisboards.at(active_id);
	//Handle Single Stroke Input 
	while ( SDL_PollEvent(&event) ) {
		switch(event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_SPACE:
						active_tetrisboard->sink_active();
						break;
					case SDLK_w:
					case SDLK_UP:
						active_tetrisboard->rotate_active();
						break;
					case SDLK_a:
					case SDLK_LEFT:
						if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]) {
							select_previous_tetrisboard();
						} else {
							active_tetrisboard->move_active_left();
							last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]) {
							select_next_tetrisboard();
						} else {
							active_tetrisboard->move_active_right();
							last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_s:
					case SDLK_DOWN:
						active_tetrisboard->move_active_down();
						last_keyhold_time = SDL_GetTicks();
						break;
					case SDLK_LCTRL:
					case SDLK_RCTRL:
						switch_hold_piece();
						break;
					case SDLK_1:
						select_tetrisboard(0);
						break;
					case SDLK_2:
						select_tetrisboard(1);
						break;
					case SDLK_3:
						select_tetrisboard(2);
						break;
					case SDLK_4:
						select_tetrisboard(3);
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
		if ( (keys[SDLK_LEFT] || keys[SDLK_a]) && !(keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])) { 
			active_tetrisboard->move_active_left();
			last_keyhold_time = SDL_GetTicks();
		}
		if ( (keys[SDLK_RIGHT] || keys[SDLK_d]) && !(keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])) { 
			active_tetrisboard->move_active_right();
			last_keyhold_time = SDL_GetTicks();
		}
		if ( keys[SDLK_DOWN] || keys[SDLK_s] ) { 
			active_tetrisboard->move_active_down();
			last_keyhold_time = SDL_GetTicks();
		}
	}
	return done;
}

void TetrisBoardMultiManager::add_tetrisboard() {
	std::vector<TetrisBoard *>::iterator itr;
	int i = 0, x = 0, dx = 0;
	int y = screen_height * 0.37;
	int new_spacing = (screen_width / (tetrisboards.size() + 2)); //Make equal spacing between each tetrisboard
	TetrisBoard * T;

	if (tetrisboards.size() < 4) {
		//Update Tetrisboard coordinates
		for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
			i++;
			x = i * new_spacing;
			dx = x - (*itr)->get_xPos();
			(*itr)->add_move_animation(dx);
			(*itr)->set_cycles(-125);
		}

		//Add the new board
		x = (tetrisboards.size() + 1) * new_spacing; 
		if (tetrisboards.size() == 0) {
			T = new TetrisBoard(this, x, y, 10, 20);
		} else {
			T = new TetrisBoard(*(tetrisboards.front())); //Make a copy of the first board
			T->set_xPos(x);
			T->set_yPos(y);
		}
		T->set_cycles(-125);
		tetrisboards.push_back(T);
		board_locks.push_back(false);
	}
}

void TetrisBoardMultiManager::remove_tetrisboard(TetrisBoard * t) {
	int i = 0, x = 0, dx = 0, id = 0;
	int y = y = screen_height / 2 - screen_height / 8;
	int new_spacing = 0;
	std::vector<TetrisBoard *>::iterator itr;

	//Find id of board to remove
	i = 0;
	for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
		if ((*itr) == t) {
			id = i;
			break;
		}
		i++;
	}
	
	if (tetrisboards.size() > 1) {
		//Remove the board
		delete tetrisboards.at(id);
		tetrisboards.erase(tetrisboards.begin() + id);
		board_locks.erase(board_locks.begin()+id);
		if (id <= active_id ) {
			active_id--;
			if (active_id < 0) {
				active_id = 0;
			}
		}

		//Update Tetrisboard coordinates
	    new_spacing = (screen_width / (tetrisboards.size() + 1)); //Make equal spacing between each tetrisboard
		i = 0;
		for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
			i++;
			x = i * new_spacing; 
			dx = x - (*itr)->get_xPos();
			(*itr)->add_move_animation(dx);
			(*itr)->set_cycles(-125);
		}
	} else {
		game_over = true;
	}
}

void TetrisBoardMultiManager::draw() {
	
	int i=0;
	int x=0;
	//int y = screen_height / 2 - screen_height / 8;
	int num_boards = (int) tetrisboards.size();
	int square_size = screen_height/35 - (num_boards);
	int active_square_size = square_size * 1.12;
	int r, g, b;
	bool allLocked = allBoardsLocked();

	
	for(std::vector<TetrisBoard *>::iterator itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
		i++;

		if (allLocked) { //Purple
			r = 255; g = 0; b = 255;
		} else {
			if (board_locks.at(i-1)) { // green if locked
				r = 0; g = 255; b = 0;
			} else {
				r = 255; g = 255; b = 255;
			}
		}
		if (i-1 == active_id) {
			(*itr)->draw(active_square_size, r, g, b);
		} else {
			(*itr)->draw(square_size,r,g,b);
		}

		gui->draw_int(i, (*itr)->get_xPos(), num_boards*5, r, g, b);
	}
	draw_hold_box(hold_piece, hold_used, 20, 700, screen_height - 120);
}

void TetrisBoardMultiManager::draw_hud() {
	int pixels_per_second = 500/(timer_duration/1000);
	if (allBoardsLocked()) {
		gui->draw_multiplier(multiplier, timer, 255,0,255, pixels_per_second);
	} else {
		gui->draw_multiplier(multiplier, timer, 0,255,0, pixels_per_second);
	}
	
	gui->draw_string("Hold", 718, 560, 255, 255, 255);
	//gui->draw_string("Level ", 10, 0, 255, 255, 255);
	//gui->draw_int(level, 60, 0, 255, 255, 255, 2, ' ');
	//gui->draw_int(score, 670, 550, 255, 255, 255);
	//gui->draw_string("Lines: ", 700, 0, 255, 255, 255);
	//gui->draw_int(lines, 760, 0, 255, 255, 255);

	////Bottom Left
	//gui->draw_string("Level ", 10, 135, 255, 128, 0);
	//gui->draw_int(level, 35, 110, 255, 255, 255);
	//gui->draw_string("Lines ", 10, 85, 255, 128, 0);
	//gui->draw_int(lines, 35, 60, 255, 255, 255);
	//gui->draw_string("Score ", 10, 35, 255, 128, 0);
	//gui->draw_int(score, 35, 10, 255, 255, 255);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor4f(0.0, 0.0, 0.0, 0.6f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glBegin(GL_QUADS);
	//	glVertex2i(625, 575);
	//	glVertex2i(690, 575);
	//	glVertex2i(690, 450);
	//	glVertex2i(625, 450);
	//glEnd();

	////Top Right
	//gui->draw_string("Level ", 630, 550, 255, 128, 0);
	//gui->draw_int(level, 655, 525, 255, 255, 255);
	//gui->draw_string("Lines ", 630, 500, 255, 128, 0);
	//gui->draw_int(lines, 655, 475, 255, 255, 255);
	//gui->draw_string("Score ", 630, 450, 255, 128, 0);
	//gui->draw_int(score, 655, 425, 255, 255, 255);

	//Top Left
	gui->draw_string("Lines ", 30, 550, 255, 128, 0);
	gui->draw_int(lines, 55, 525, 255, 255, 255);
	gui->draw_string("Score ", 30, 500, 255, 128, 0);
	gui->draw_int(score, 55, 475, 255, 255, 255);
}

void TetrisBoardMultiManager::draw_controls() {
	gui->draw_string("Multi", 375, 550, 0, 255, 0);

	gui->draw_string("Controls", 50, 500, 255, 128, 0);

	gui->draw_string("Rotate", 75, 450, 255, 255, 255);
	gui->draw_string("Move Left", 75, 420, 255, 255, 255);
	gui->draw_string("Move Down", 75, 390, 255, 255, 255);
	gui->draw_string("Move Right", 75, 360, 255, 255, 255);
	gui->draw_string("Drop", 75, 330, 255, 255, 255);
	gui->draw_string("Hold", 75, 300, 255, 255, 255);
	gui->draw_string("Switch Board", 75, 270, 255, 255, 255);
	gui->draw_string("Quit", 75, 240, 255, 255, 255);

	gui->draw_string("Up or W", 250, 450, 255, 255, 255);
	gui->draw_string("Left or A", 250, 420, 255, 255, 255);
	gui->draw_string("Down or S", 250, 390, 255, 255, 255);
	gui->draw_string("Right or D", 250, 360, 255, 255, 255);
	gui->draw_string("Space Bar", 250, 330, 255, 255, 255);
	gui->draw_string("Control", 250, 300, 255, 255, 255);
	gui->draw_string("1/2/3/4 or Shift + Left/Right", 250, 270, 255, 255, 255);
	gui->draw_string("Escape", 250, 240, 255, 255, 255);

	gui->draw_string("Hints", 50, 180, 255, 128, 0);
	gui->draw_string("+ Clear a line to lock the active board.", 75, 130, 255, 255 ,255);
	gui->draw_string("+ Clear a line while all boards are locked to clone the active board.", 75, 100, 255, 255 ,255);
	gui->draw_string("+ Time your clears carefully. Your boards will unlock if the timer runs out.", 75, 70, 255, 255 ,255);
	gui->draw_string("+ Control four boards at once for maximum points!", 75, 40, 255, 255 ,255);

	gui->draw_string("Press Enter to Start!", 600, 10, 255, 255 ,255);
}

void TetrisBoardMultiManager::draw_game_over() {
	gui->draw_string("GAME OVER!", 50, 200, 255, 255, 255);
	gui->draw_string("Your score: ", 50, 280, 255, 255, 255);
	gui->draw_int(score, 170, 280, 255, 255, 255);
	gui->draw_string("Press Enter to continue...", 540, 10, 255, 255, 255);
}

void TetrisBoardMultiManager::switch_hold_piece() {
	if (!hold_used) {
		TetrisBoard * tb = tetrisboards.at(active_id);
		TetrisPiece * temp = hold_piece;
		hold_piece = tb->get_active_piece();
		tb->set_active_piece(temp);
		hold_used = true;
	}
}

void TetrisBoardMultiManager::select_previous_tetrisboard() {
	active_id--;
	if (active_id < 0) {
		active_id = (int) tetrisboards.size() - 1;
	}
}
void TetrisBoardMultiManager::select_next_tetrisboard() {
	active_id++;
	if (active_id >= tetrisboards.size()) {
		active_id = 0;
	}
}

void TetrisBoardMultiManager::select_tetrisboard(int id) {
	if (id < tetrisboards.size()) {
		active_id = id;
	}
}

//Reporters

void TetrisBoardMultiManager::report_piece_drop(TetrisBoard * T) {
	std::vector<TetrisBoard *>::iterator itr;
	score += 10 * multiplier;
	hold_used = false;
	
	////Move down all boards besides the active one

	//int i =0;
	//for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
	//	if (i != active_id) {
	//		(*itr)->set_cycles(116);
	//	}
	//	i++;
	//}
}
void TetrisBoardMultiManager::report_line_clear(TetrisBoard * T, int num_lines) {
	std::stringstream ss;
	std::string string_type, string_score, string_multiplier;
	std::vector<TetrisBoard *>::iterator itr;

	int clear_score;
	
	timer_duration = 1000 * (3 * tetrisboards.size() - chain_count);
	timer = timer_duration;

	if (allBoardsLocked())  {
		unlockBoards();
		if (tetrisboards.size() < 4) {
			num_boards_to_add++;
			timer = 0;
			multiplier = tetrisboards.size();
			gui->add_text_animation("Clone activated!", 325, 400, 150);
		} else {
			chain_count = 0;
			multiplier *= 2;
			gui->add_text_animation("Double Multiplier!!", 325, 400, 150);
		}
	} else {
		//Lock board
		int i =0;
		for(itr = tetrisboards.begin(); itr != tetrisboards.end(); itr++) {
			if (T == tetrisboards.at(i) && !board_locks.at(i)) {
				board_locks.at(i) = true;
			}
			i++;
		}
	}

	switch (num_lines){
		case 1: 
			string_type = "SINGLE!";
			if (chain_count > 0) {
				//multiplier += 1;
			}
			break;
		case 2:
			string_type = "DOUBLE!!";
			if (chain_count > 0) {
				//multiplier += 2;
			}
			break;
		case 3:
			string_type = "TRIPLE!!!";
			if (chain_count > 0) {
				//multiplier += 4;
			}
			break;
		case 4:
			string_type = "CHEZ-TRIS!!!?";
			if (chain_count > 0) {
				//multiplier += 8;
			}
			break;
	}

	clear_score = (num_lines * num_lines * 100) * multiplier;
	ss << clear_score;
	ss >> string_score;
	//ss.clear();

	score += clear_score;
	lines += num_lines;
	level = lines / 10 + 1;
	if (level > 15) {
		level = 15;
	}

	/*
	if (chain_count > 0) {
		ss << multiplier;
		ss >> string_multiplier;
		string_multiplier = " x" + string_multiplier;
	} else {
		string_multiplier = "";
	}*/

	chain_count++;
	gui->add_text_animation(string_type + " +" + string_score, T->get_xPos()-50, T->get_yPos(), 75);

}
bool TetrisBoardMultiManager::allBoardsLocked() {
	//Check if all boards are locked
	for(std::vector<bool>::iterator itr = board_locks.begin(); itr != board_locks.end(); itr++) {
		if (!(*itr)) {
			return false;
		}
	}
	return true;
}

void TetrisBoardMultiManager::unlockBoards() {
	for(std::vector<bool>::iterator itr = board_locks.begin(); itr != board_locks.end(); itr++) {
		*itr = false;
	}
}