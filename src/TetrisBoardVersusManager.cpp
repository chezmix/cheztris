#include "TetrisBoardVersusManager.h"
#include "TetrisBoard.h"
#include "SquarePiece.h"
#include "SuperSquarePiece.h"
#include "Gui.h"
#include <ctime> // for random numbers

TetrisBoardVersusManager::TetrisBoardVersusManager(Gui * g) :
gui(g),
p1_hold_piece(NULL),
p1_hold_used(false),
p2_hold_piece(NULL),
p2_hold_used(false),
p1_meter(0),
p2_meter(0),
p1_last_keyhold_time(0),
p2_last_keyhold_time(0),
p1_item(0),
p2_item(0),
p1_effect(0),
p2_effect(0),
p1_effect_duration(0),
p2_effect_duration(0),
num_items_used(0)
{
	p1 = new TetrisBoard(this, screen_width * 0.30, screen_height/2 - screen_height/8, 10, 20);
	p2 = new TetrisBoard(this, screen_width * 0.70, screen_height/2 - screen_height/8, 10, 20);
}

TetrisBoardVersusManager::~TetrisBoardVersusManager()
{
	delete p1;
	delete p2;
}

void TetrisBoardVersusManager::cycle() {
	int p1_speed, p2_speed;
	if (p1_effect_duration == 0) {
		p1_effect = 0;
	}
	if (p2_effect_duration == 0) {
		p2_effect = 0;
	}

	p1_speed = min(10+ num_items_used * 5, 95);
	p2_speed = min(10+ num_items_used * 5, 95);

	if (p1_effect == SPEED) {
		p1_speed = min(max(10+ num_items_used * 10,50),100);
	}
	if (p2_effect == SPEED) {
		p2_speed = min(max(10+ num_items_used * 10,50), 100);
	}
	if (p1_effect == SUPERSPEED) {
		p1_speed = min(max(10+ num_items_used * 15,85), 103);
	}
	if (p2_effect == SUPERSPEED) {
		p2_speed = min(max(10+ num_items_used * 15,85), 103);
	}

	p1->set_speed(p1_speed);
	p2->set_speed(p2_speed);
	gui->cycle();	
	p1->cycle();
	p2->cycle();
	if (p1->is_dead() || p2->is_dead()) {
		game_over = true;
	}
}

bool TetrisBoardVersusManager::handle_input() {
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
						if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
							p1->sink_active();
						}
						break;
					case SDLK_RSHIFT:
						if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
							p2->sink_active();
						}
						break;
					case SDLK_w:
						if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
							p1->rotate_active();
						}
						break;
					case SDLK_UP:
						if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
							p2->rotate_active();
						}
						break;
					case SDLK_a:
						if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
							p1->move_active_left();
							p1_last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_LEFT:
						if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
							p2->move_active_left();
							p2_last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_d:
						if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
							p1->move_active_right();
							p1_last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_RIGHT:
						if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
							p2->move_active_right();
							p2_last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_s:
						if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
							p1->move_active_down();
							p1_last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_DOWN:
						if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
							p2->move_active_down();
							p2_last_keyhold_time = SDL_GetTicks();
						}
						break;
					case SDLK_LCTRL:
						if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
							p1_switch_hold_piece();
						}
						break;
					case SDLK_RCTRL:
						if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
							p2_switch_hold_piece();
						}
						break;
					case SDLK_e:
						p1_activate_item();
						break;
					case SDLK_RETURN:
						p2_activate_item();
						break;
				}
				break;
			case SDL_QUIT:
				done = true;
				break;
		}
	} // end input


	//Handle Continuous Input
	if ((p1_effect != STUN) && (p1_effect != SUPERSTUN)) {
		if (SDL_GetTicks() - p1_last_keyhold_time > KEY_HOLD_DELAY) {
			if ( keys[SDLK_a] ) { 
				p1->move_active_left();
				p1_last_keyhold_time = SDL_GetTicks();
			}
			if ( keys[SDLK_d] ) { 
				p1->move_active_right();
				p1_last_keyhold_time = SDL_GetTicks();
			}
			if ( keys[SDLK_s] ) { 
				p1->move_active_down();
				p1_last_keyhold_time = SDL_GetTicks();
			}
		}
	}

	if ((p2_effect != STUN) && (p2_effect != SUPERSTUN)) {
		if (SDL_GetTicks() - p2_last_keyhold_time > KEY_HOLD_DELAY) {
			if ( keys[SDLK_LEFT] ) { 
				p2->move_active_left();
				p2_last_keyhold_time = SDL_GetTicks();
			}
			if ( keys[SDLK_RIGHT] ) { 
				p2->move_active_right();
				p2_last_keyhold_time = SDL_GetTicks();
			}
			if ( keys[SDLK_DOWN] ) { 
				p2->move_active_down();
				p2_last_keyhold_time = SDL_GetTicks();
			}
		}
	}
	return done;
}

void TetrisBoardVersusManager::draw() {
	int square_size = 18;
	p1->draw(square_size,1.0,1.0,1.0);
	p2->draw(square_size,1.0,1.0,1.0);
	draw_hold_box(p1_hold_piece, p1_hold_used, square_size, screen_width/2 - 250, screen_height - 158);
	draw_hold_box(p2_hold_piece, p2_hold_used, square_size, screen_width/2 + 70, screen_height - 158);
	draw_meters();
}

void TetrisBoardVersusManager::draw_hud() {
	gui->draw_string("Hold", 165, 515, 255, 255, 255);
	gui->draw_string("Next", 270, 515, 255, 255, 255);
	gui->draw_string("Attack", 10, 515, 255, 255, 255);
	gui->draw_string("Hold", 485, 515, 255, 255, 255);
	gui->draw_string("Next", 590, 515, 255, 255, 255);
	gui->draw_string("Attack", 732, 515, 255, 255, 255);
	//gui->draw_string("Level ", 10, 0, 255, 255, 255);
	//gui->draw_int(p1_level, 60, 0, 255, 255, 255);
	//gui->draw_string("Lines: ", 700, 0, 255, 255, 255);
	//gui->draw_int(p1_lines, 760, 0, 255, 255, 255);
}

void TetrisBoardVersusManager::draw_controls() {
	gui->draw_string("Versus", 375, 550, 0, 255, 0);

	gui->draw_string("1P Controls", 50, 500, 255, 128, 0);

	gui->draw_string("Rotate", 75, 450, 255, 255, 255);
	gui->draw_string("Move Left", 75, 420, 255, 255, 255);
	gui->draw_string("Move Down", 75, 390, 255, 255, 255);
	gui->draw_string("Move Right", 75, 360, 255, 255, 255);
	gui->draw_string("Drop", 75, 330, 255, 255, 255);
	gui->draw_string("Hold", 75, 300, 255, 255, 255);
	gui->draw_string("Attack", 75, 270, 255, 255, 255);

	gui->draw_string("W", 250, 450, 255, 255, 255);
	gui->draw_string("A", 250, 420, 255, 255, 255);
	gui->draw_string("S", 250, 390, 255, 255, 255);
	gui->draw_string("D", 250, 360, 255, 255, 255);
	gui->draw_string("Space Bar", 250, 330, 255, 255, 255);
	gui->draw_string("L-Control", 250, 300, 255, 255, 255);
	gui->draw_string("E", 250, 270, 255, 255, 255);

	gui->draw_string("2P Controls", 475, 500, 255, 128, 0);

	gui->draw_string("Rotate", 500, 450, 255, 255, 255);
	gui->draw_string("Move Left", 500, 420, 255, 255, 255);
	gui->draw_string("Move Down", 500, 390, 255, 255, 255);
	gui->draw_string("Move Right", 500, 360, 255, 255, 255);
	gui->draw_string("Drop", 500, 330, 255, 255, 255);
	gui->draw_string("Hold", 500, 300, 255, 255, 255);
	gui->draw_string("Attack", 500, 270, 255, 255, 255);

	gui->draw_string("Up", 675, 450, 255, 255, 255);
	gui->draw_string("Left", 675, 420, 255, 255, 255);
	gui->draw_string("Down", 675, 390, 255, 255, 255);
	gui->draw_string("Right", 675, 360, 255, 255, 255);
	gui->draw_string("R-Shift", 675, 330, 255, 255, 255);
	gui->draw_string("R-Control", 675, 300, 255, 255, 255);
	gui->draw_string("Enter", 675, 270, 255, 255, 255);


	gui->draw_string("Hints", 50, 200, 255, 128, 0);
	gui->draw_string("+ Clearing lines will fill up your attack meter.", 75, 150, 255, 255 ,255);
	gui->draw_string("+ Fill up your meter once to gain an attack, or twice to gain a super attack.", 75, 120, 255, 255 ,255);
	gui->draw_string("+ Time your attacks wisely to crush your opponent!", 75, 90, 255, 255 ,255);

	gui->draw_string("Press Enter to Start!", 600, 10, 255, 255 ,255);
}

void TetrisBoardVersusManager::draw_game_over() {
	if (p2->is_dead()) {
		gui->draw_string("Player 1 Wins!", 50, 200, 255, 255, 255);
	} else {
		gui->draw_string("Player 2 Wins!", 50, 200, 255, 255, 255);
	}
	gui->draw_string("Press Enter to continue...", 540, 10, 255, 255, 255);
}

void TetrisBoardVersusManager::p1_switch_hold_piece() {
	if (!p1_hold_used) {
		TetrisPiece * temp = p1_hold_piece;
		p1_hold_piece = p1->get_active_piece();
		p1->set_active_piece(temp);
		p1_hold_used = true;
	}
}

void TetrisBoardVersusManager::p2_switch_hold_piece() {
	if (!p2_hold_used) {
		TetrisPiece * temp = p2_hold_piece;
		p2_hold_piece = p2->get_active_piece();
		p2->set_active_piece(temp);
		p2_hold_used = true;
	}
}


//Reporters

void TetrisBoardVersusManager::report_piece_drop(TetrisBoard * T) {
	if (is_p1_board(T)) {
		p1_hold_used = false;
		if (p1_effect_duration > 0) {
			p1_effect_duration--;
		}
	} else {
		p2_hold_used = false;
		if (p2_effect_duration > 0) {
			p2_effect_duration--;
		}
	}
}
void TetrisBoardVersusManager::report_line_clear(TetrisBoard * T, int num_lines) {
	std::string string_text;
	int meter_gain;

	switch (num_lines){
		case 1: 
			string_text = "+2";
			meter_gain = 2;
			break;
		case 2:
			string_text = "+5";
			meter_gain = 5;
			break;
		case 3:
			string_text = "+10";
			meter_gain = 10;
			break;
		case 4:
			string_text = "+20";
			meter_gain = 20;
			break;
	}

	if (is_p1_board(T)) {
		p1_meter += meter_gain;
		if (p1_meter >= 20 && p1_item == NOITEM) {
			p1_item = (rand() % 3) + 1; //Generate integer between 1 - 3
		}

		if (p1_meter >= 40) {
			p1_meter = 40;
			switch (p1_item) { //Upgrade!
				case SPEED:
					p1_item = SUPERSPEED;
					break;
				case STUN:
					p1_item = SUPERSTUN;
					break;
				case SQUARE:
					p1_item = SUPERSQUARE;
					break;
				default:
					break;
			}
		}

	} else {
		p2_meter += meter_gain;
		if (p2_meter >= 20 && p2_item == NOITEM) {
			p2_item = (rand() % 3) + 1; //Generate integer between 1 - 3
		}

		if (p2_meter >= 40) {
			p2_meter = 40;
			switch (p2_item) { //Upgrade!
				case SPEED:
					p2_item = SUPERSPEED;
					break;
				case STUN:
					p2_item = SUPERSTUN;
					break;
				case SQUARE:
					p2_item = SUPERSQUARE;
					break;
				default:
					break;
			}
		}
	}

	gui->add_text_animation(string_text, T->get_xPos(), T->get_yPos(), 75);
}

//Private
bool TetrisBoardVersusManager::is_p1_board(TetrisBoard * T) {
	if (T->get_xPos() < screen_width/2) {
		return true;
	} else {
		return false;
	}
}

void TetrisBoardVersusManager::draw_meters() {
	/**************** Player 1 ****************/
	//Meter Cover
	glColor4f(0.0, 0.0, 0.0, 0.75f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		glVertex2f(5, 45);
		glVertex2f(23, 45);
		glVertex2f(23, 404);
		glVertex2f(5, 404);
	glEnd();

	//Meter Outline
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex2f(5, 45);
		glVertex2f(23, 45);
		glVertex2f(23, 404);
		glVertex2f(5, 404);
	glEnd();

	//Lv 1 Meter Blocks
	glColor4f(0.0, 1.0, 0.0, 0.8f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		for (int i = 0; i < 20 && i < p1_meter; i++) {
			glVertex2f(5, 45 + 18 * i);
			glVertex2f(23, 45 + 18 * i);
			glVertex2f(23, 43 + 18 * (i+1));
			glVertex2f(5, 43 + 18 * (i+1));
		}
	glEnd();
	
	//Lv 2 Meter Blocks
	glColor4f(1.0, 0.0, 1.0, 0.8f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		for (int i = 0; i < 20 && i < p1_meter - 20; i++) {
			glVertex2f(5, 45 + 18 * i);
			glVertex2f(23, 45 + 18 * i);
			glVertex2f(23, 43 + 18 * (i+1));
			glVertex2f(5, 43 + 18 * (i+1));
		}
	glEnd();

	//Item Cover
	glColor4f(0.0, 0.0, 0.0, 0.75f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		glVertex2f(5, 443);
		glVertex2f(75, 443);
		glVertex2f(75, 514);
		glVertex2f(5, 514);
	glEnd();

	//Item Outline
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex2f(5, 443);
		glVertex2f(75, 443);
		glVertex2f(75, 514);
		glVertex2f(5, 514);
	glEnd();

	/*************** Player 2 ****************/
	//Meter Cover
	glColor4f(0.0, 0.0, 0.0, 0.75f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		glVertex2f(777, 45);
		glVertex2f(795, 45);
		glVertex2f(795, 404);
		glVertex2f(777, 404);
	glEnd();

	//Meter Outline
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex2f(777, 45);
		glVertex2f(795, 45);
		glVertex2f(795, 404);
		glVertex2f(777, 404);
	glEnd();

	//Lv 1 Meter Blocks
	glColor4f(0.0, 1.0, 0.0, 0.8f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		for (int i = 0; i < 20 && i < p2_meter; i++) {
			glVertex2f(777, 45 + 18 * i);
			glVertex2f(795, 45 + 18 * i);
			glVertex2f(795, 43 + 18 * (i+1));
			glVertex2f(777, 43 + 18 * (i+1));
		}
	glEnd();

	//Lv 2 Meter Blocks
	glColor4f(1.0, 0.0, 1.0, 0.8f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		for (int i = 0; i < 20 && i < p2_meter - 20; i++) {
			glVertex2f(777, 45 + 18 * i);
			glVertex2f(795, 45 + 18 * i);
			glVertex2f(795, 43 + 18 * (i+1));
			glVertex2f(777, 43 + 18 * (i+1));
		}
	glEnd();

	//Item Cover
	glColor4f(0.0, 0.0, 0.0, 0.75f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		glVertex2f(725, 443);
		glVertex2f(795, 443);
		glVertex2f(795, 514);
		glVertex2f(725, 514);
	glEnd();

	//Item Outline
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex2f(725, 443);
		glVertex2f(795, 443);
		glVertex2f(795, 514);
		glVertex2f(725, 514);
	glEnd();

	//Display the items
	switch (p1_item) {
		case SPEED:
			gui->draw_string("Speed", 10, 465, 255, 0, 0);
			break;
		case STUN:
			gui->draw_string("Stun", 20, 465, 255, 0, 0);
			break;
		case SQUARE:
			gui->draw_string("Square", 8, 465, 255, 0, 0);
			break;
		case SUPERSPEED:
			gui->draw_string("Super", 14, 470, 255, 0, 0);
			gui->draw_string("Speed", 10, 450, 255, 0, 0);
			break;
		case SUPERSTUN:
			gui->draw_string("Super", 14, 470, 255, 0, 0);
			gui->draw_string("Stun", 17, 450, 255, 0, 0);
			break;
		case SUPERSQUARE:
			gui->draw_string("Super", 14, 470, 255, 0, 0);
			gui->draw_string("Square", 8, 450, 255, 0, 0);
			break;
		default:
			break;
	}

	switch (p2_item) {
		case SPEED:
			gui->draw_string("Speed", 730, 465, 255, 0, 0);
			break;
		case STUN:
			gui->draw_string("Stun", 740, 465, 255, 0, 0);
			break;
		case SQUARE:
			gui->draw_string("Square", 728, 465, 255, 0, 0);
			break;
		case SUPERSPEED:
			gui->draw_string("Super", 734, 470, 255, 0, 0);
			gui->draw_string("Speed", 730, 450, 255, 0, 0);
			break;
		case SUPERSTUN:
			gui->draw_string("Super", 734, 470, 255, 0, 0);
			gui->draw_string("Stun", 737, 450, 255, 0, 0);
			break;
		case SUPERSQUARE:
			gui->draw_string("Super", 734, 470, 255, 0, 0);
			gui->draw_string("Square", 728, 450, 255, 0, 0);
			break;
		default:
			break;
	}
}

void TetrisBoardVersusManager::p1_activate_item() {
	switch (p1_item) {
		case SPEED:
			p1_item = NOITEM;
			p2_effect = SPEED;
			p2_effect_duration = 5;
			num_items_used++;
			p1_meter = 0;
			gui->add_text_animation("+Speed", p2->get_xPos(), p2->get_yPos(), 75);
			break;
		case STUN:
			p1_item = NOITEM;
			p2_effect = STUN;
			p2_effect_duration = 1;
			num_items_used++;
			p1_meter = 0;
			gui->add_text_animation("+Stun", p2->get_xPos(), p2->get_yPos(), 75);
			break;
		case SQUARE:
			p1_item = NOITEM;
			p2->set_next_piece(new SquarePiece());
			num_items_used++;
			p1_meter = 0;
			gui->add_text_animation("+Square", p2->get_xPos(), p2->get_yPos(), 75);
			break;
		case SUPERSPEED:
			p1_item = NOITEM;
			p2_effect = SUPERSPEED;
			p2_effect_duration = 8;
			num_items_used++;
			p1_meter = 0;
			gui->add_text_animation("+Super Speed", p2->get_xPos(), p2->get_yPos(), 75);
			break;
		case SUPERSTUN:
			p1_item = NOITEM;
			p2_effect = SUPERSTUN;
			p2_effect_duration = 2;
			num_items_used++;
			p1_meter = 0;
			gui->add_text_animation("+Super Stun", p2->get_xPos(), p2->get_yPos(), 75);
			break;
		case SUPERSQUARE:
			p1_item = NOITEM;
			p2->set_next_piece(new SuperSquarePiece());
			num_items_used++;
			p1_meter = 0;
			gui->add_text_animation("+Super Square", p2->get_xPos(), p2->get_yPos(), 75);
			break;
		default:
			break;
	}

}

void TetrisBoardVersusManager::p2_activate_item() {
	switch (p2_item) {
		case SPEED:
			p2_item = NOITEM;
			p1_effect = SPEED;
			p1_effect_duration = 5;
			num_items_used++;
			p2_meter = 0;
			gui->add_text_animation("+Speed", p1->get_xPos(), p2->get_yPos(), 75);
			break;
		case STUN:
			p2_item = NOITEM;
			p1_effect = STUN;
			p1_effect_duration = 1;
			num_items_used++;
			p2_meter = 0;
			gui->add_text_animation("+Stun", p1->get_xPos(), p2->get_yPos(), 75);
			break;
		case SQUARE:
			p2_item = NOITEM;
			p1->set_next_piece(new SquarePiece());
			num_items_used++;
			p2_meter = 0;
			gui->add_text_animation("+Square", p1->get_xPos(), p2->get_yPos(), 75);
			break;
		case SUPERSPEED:
			p2_item = NOITEM;
			p1_effect = SUPERSPEED;
			p1_effect_duration = 8;
			num_items_used++;
			p2_meter = 0;
			gui->add_text_animation("+Super Speed", p1->get_xPos(), p2->get_yPos(), 75);
			break;
		case SUPERSTUN:
			p2_item = NOITEM;
			p1_effect = SUPERSTUN;
			p1_effect_duration = 2;
			num_items_used++;
			p2_meter = 0;
			gui->add_text_animation("+Super Stun", p1->get_xPos(), p2->get_yPos(), 75);
			break;
		case SUPERSQUARE:
			p2_item = NOITEM;
			p1->set_next_piece(new SuperSquarePiece());
			num_items_used++;
			p2_meter = 0;
			gui->add_text_animation("+Super Square", p1->get_xPos(), p2->get_yPos(), 75);
			break;
		default:
			break;
	}
}