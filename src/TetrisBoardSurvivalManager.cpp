#include "TetrisBoardSurvivalManager.h"
#include "TetrisBoard.h"
#include "Gui.h"
#include <ctime> // for random numbers

TetrisBoardSurvivalManager::TetrisBoardSurvivalManager(Gui * g) :
gui(g),
hold_piece(NULL),
hold_used(false),
lines(0),
last_keyhold_time(0),
attack_delay(10000),
start_time(0),
elapsed_timer(0),
attack_start_time(0),
attack_timer(0),
bonus_time(0),
save_threshold(2000),
speed(10)
{
	the_board = new TetrisBoard(this, screen_width/2, screen_height/2 - screen_height/8, 10, 20);
}

TetrisBoardSurvivalManager::~TetrisBoardSurvivalManager()
{
	delete the_board;
}

void TetrisBoardSurvivalManager::cycle() {
	gui->cycle();	
	the_board->cycle();
	if (start_time == 0) {
		start_time = SDL_GetTicks();
		attack_start_time = start_time;
	}
	elapsed_timer = SDL_GetTicks() - start_time;
	if (the_board->is_dead()) {
		game_over = true;
	}

	//When to Attack
	attack_timer = attack_delay - (SDL_GetTicks() - attack_start_time) + bonus_time;
	if (attack_timer <= 0) {
		attack();
	}
}

bool TetrisBoardSurvivalManager::handle_input() {
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

void TetrisBoardSurvivalManager::draw() {
	int square_size = 20;
	int hold_x = 300;
	int hold_y = 465;
	bool is_active = false;
	if (attack_timer < save_threshold) {
		the_board->draw(square_size,1.0,0.0,0.0);
	} else {
		the_board->draw(square_size,1.0,1.0,1.0);
	}
	
	//the_board->set_board_height(25);
	draw_hold_box(hold_piece, hold_used, square_size, hold_x, hold_y);
}

void TetrisBoardSurvivalManager::draw_hud() {
	gui->draw_string("Hold", 310, 555, 255, 255, 255);
	gui->draw_string("Next", 435, 555, 255, 255, 255);

	gui->draw_string("Time", 650, 555, 255, 255, 255);
	gui->draw_int(elapsed_timer/60000, 700, 555, 255, 255, 255, 2, '0'); //Minutes
	gui->draw_int((elapsed_timer % 60000)/1000, 725, 555, 255, 255, 255, 2, '0'); //Seconds
	gui->draw_int((elapsed_timer % 1000)/10, 750, 555, 255, 255, 255, 2, '0');  //Centiseconds
	gui->draw_string(":", 720, 555, 255, 255, 255);
	gui->draw_string(":", 746, 555, 255, 255, 255);

	gui->draw_string("Next Attack", 50, 400, 255, 128, 0);
	if (attack_timer < save_threshold) { //draw attack timer red
		gui->draw_int((attack_timer%60000)/1000, 200, 400, 255, 0, 0, 2, '0');  //Seconds
		gui->draw_int((attack_timer%1000)/10, 228, 400, 255, 0, 0); //Centiseconds
		gui->draw_string(":", 222, 400, 255, 128, 0);
	} else { //draw attack timer white
		gui->draw_int((attack_timer%60000)/1000, 200, 400, 255, 255, 255, 2, '0');  //Seconds
		gui->draw_int((attack_timer%1000)/10, 228, 400, 255, 255, 255); //Centiseconds
		gui->draw_string(":", 222, 400, 255, 255, 255);
	}
}

void TetrisBoardSurvivalManager::draw_controls() {
	gui->draw_string("Survival", 375, 550, 0, 255, 0);

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
	gui->draw_string("+ When the attack timer reaches 0 you will be hit with a random attack.", 75, 150, 255, 255 ,255);
	gui->draw_string("+ Clearing lines will delay attacks.", 75, 120, 255, 255 ,255);
	gui->draw_string("+ Clear a line while the board glows red to gain double the time bonus!", 75, 90, 255, 255 ,255);

	gui->draw_string("Press Enter to Start!", 600, 10, 255, 255 ,255);
}

void TetrisBoardSurvivalManager::draw_game_over() {
	gui->draw_string("GAME OVER!", 50, 200, 255, 255, 255);
	gui->draw_string("Your Time: ", 50, 280, 255, 255, 255);
	gui->draw_int(elapsed_timer/60000, 170, 280, 255, 255, 255, 2, '0'); //Minutes
	gui->draw_int((elapsed_timer % 60000)/1000, 195, 280, 255, 255, 255, 2, '0'); //Seconds
	gui->draw_int((elapsed_timer % 1000)/10, 220, 280, 255, 255, 255, 2, '0');  //Centiseconds
	gui->draw_string(":", 190, 280, 255, 255, 255);
	gui->draw_string(":", 216, 280, 255, 255, 255);
	gui->draw_string("Press Enter to continue...", 540, 10, 255, 255, 255);
}

void TetrisBoardSurvivalManager::switch_hold_piece() {
	if (!hold_used) {
		TetrisPiece * temp = hold_piece;
		hold_piece = the_board->get_active_piece();
		the_board->set_active_piece(temp);
		hold_used = true;
	}
}

void TetrisBoardSurvivalManager::attack() {
	int square_size = 20;
	int attack_type;
	attack_start_time = SDL_GetTicks();
	bonus_time = 0;
	
	attack_type = (rand() % 3) + 1; //Generate integer between 1 - 3
	switch (attack_type) {
		case ATTACK_SPEED:
			speed += 10;
			if (speed > 105) {
				speed = 105;
			}
			the_board->set_speed(speed);
			gui->add_text_animation("+Block Speed", 100, 100, 200);
			break;
		case ATTACK_TIMER:
			attack_delay -= 500;
			gui->add_text_animation("+Attack Speed", 100, 100, 200);
			break;
		case ATTACK_SHRINK:
			gui->add_text_animation("-Size", 100, 100, 200);
			the_board->decrease_board_height();
			the_board->set_yPos(the_board->get_yPos()+10);
			break;
		default:
			break;
	}
}


//Reporters

void TetrisBoardSurvivalManager::report_piece_drop(TetrisBoard * T) {
	hold_used = false;
}
void TetrisBoardSurvivalManager::report_line_clear(TetrisBoard * T, int num_lines) {
	std::string bonus_string;
	int save_multiplier = 1;

	if (attack_timer < save_threshold) {
		//Add Save Bonus
		save_multiplier = 2;
	}

	switch (num_lines){
		case 1: 
			bonus_string = "+1.0s";
			bonus_time += 1000 * save_multiplier;
			break;
		case 2:
			bonus_string = "+3.0s";
			bonus_time += 3000 * save_multiplier;
			break;
		case 3:
			bonus_string = "+6.0s";
			bonus_time += 6000 * save_multiplier;
			break;
		case 4:
			bonus_string = "+10.0s";
			bonus_time += 10000 * save_multiplier;
			break;
	}

	if (attack_timer < save_threshold) {
		gui->add_text_animation(bonus_string, the_board->get_xPos(), the_board->get_yPos()+20, 75);		
	}
	gui->add_text_animation(bonus_string, the_board->get_xPos(), the_board->get_yPos(), 75);
}