//Grid coordinates start with the top left of the board at 0,0

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SDL/SDL.h>
#include "TetrisBoard.h"
#include "TetrisPiece.h"
#include "TetrisBoardManager.h"
#include "SPiece.h"
#include "BoxPiece.h"
#include "LongPiece.h"
#include "LPiece.h"
#include "JPiece.h"
#include "TwoPiece.h"
#include "TPiece.h"


TetrisBoard::TetrisBoard(TetrisBoardManager * tbm, int x, int y, int width, int height):
xPos(x),
yPos(y),
active_piece_x(0), 
active_piece_y(0), 
speed(10),
dead(false),
active_piece(NULL),
next_piece(NULL),
TBM(tbm),
board_width(width),
board_height(height),
cycles(0),
move_animation_active(false),
move_animation_delta_x(0),
last_cycle_time(0)
{
	grid = new int[(board_width+2)*(board_height+2)]; //extra 2 for extra boundaries
	init_grid();
}

//Make a copy!
TetrisBoard::TetrisBoard(const TetrisBoard &t):
xPos(t.xPos),
yPos(t.yPos),
active_piece_x(t.active_piece_x), 
active_piece_y(t.active_piece_y), 
speed(t.speed),
dead(false),
active_piece(NULL),
next_piece(NULL),
TBM(t.TBM),
board_width(t.board_width),
board_height(t.board_height),
cycles(0),
move_animation_active(false),
move_animation_delta_x(0),
last_cycle_time(0)
{
	int gridSize = (board_width+2)*(board_height+2);
	grid = new int[gridSize];
	for (int i = 0; i < gridSize; i++) {
		grid[i] = t.grid[i];
	}
	active_piece = t.active_piece->clone();
	next_piece = t.next_piece->clone();
}

TetrisBoard::~TetrisBoard() {
	delete active_piece;
	delete next_piece;
	delete [] grid;
}


/* Draw the current configuration of the tetris board */
void TetrisBoard::draw(int square_size, float r, float g, float b) {
	int i = 0;
	int * next_state = next_piece->get_state();
	int * active_state = active_piece->get_state();
	int row, col;
	int horizontal_half_board_length = board_width  * square_size / 2;
	int vertical_half_board_length = board_height * square_size / 2;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glTranslatef(xPos, yPos, 0.0);

		//Next Box
		glPushMatrix();
			glTranslatef(horizontal_half_board_length - 4*square_size, vertical_half_board_length + 2*square_size, 0); //bottom left corner of board

			//Next Box Cover
			glColor4f(0.0, 0.0, 0.0, 0.75f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_QUADS);				
				glVertex2f(1, 4 * square_size - 1);
				glVertex2f(4 * square_size - 1, 4 * square_size - 1);
				glVertex2f(4 * square_size - 1, 1);
				glVertex2f(1, 1);
			glEnd();

			//Next Box Outline
			glColor4f(1.0, 1.0, 1.0, 0.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_QUADS);				
				glVertex2f(0, 4 * square_size);
				glVertex2f(4 * square_size, 4 * square_size);
				glVertex2f(4 * square_size, 0);
				glVertex2f(0, 0);
			glEnd();
			
			//Next Box Contents
			for (int i = 0; i < 16; i++) {
				row = 4 - (i /4);
				col = i % 4;
				if (next_state[i]) {
					draw_block(col, row, square_size, next_state[i], false);
				}
			}
		glPopMatrix();


		//Draw Board Cover
		glColor4f(0.0, 0.0, 0.0, 0.6f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
			glVertex2i(-1*horizontal_half_board_length,-1*vertical_half_board_length);
			glVertex2i(horizontal_half_board_length,-1*vertical_half_board_length);
			glVertex2i(horizontal_half_board_length,vertical_half_board_length);
			glVertex2i(-1*horizontal_half_board_length,(vertical_half_board_length));
		glEnd();

		//Draw Board Outline
		glColor4f(r, g, b, .75f);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
			glVertex2i(-1*horizontal_half_board_length,-1*vertical_half_board_length);
			glVertex2i(horizontal_half_board_length,-1*vertical_half_board_length);
			glVertex2i(horizontal_half_board_length,vertical_half_board_length);
			glVertex2i(-1*horizontal_half_board_length,(vertical_half_board_length));
		glEnd();
			
		//Draw Blocks
		glPushMatrix();
			glTranslatef(0, square_size * -2, 0); //shift down for the two 'imaginary' rows
			glTranslatef(-1*horizontal_half_board_length, -1*vertical_half_board_length, 0); //bottom left corner of board
			for (int y = board_height + 2; y > 0; y--) {
				for  (int x = 0; x < board_width + 2; x++) {
					if (i % (board_width + 2) < board_width && i < ((board_width+2)*board_height)) {
						if (grid[i]) {
							draw_block(x, y, square_size, grid[i], false);
						}
					}
					i++;
				}
			}
			draw_shadow_piece(square_size);
		glPopMatrix();

	glPopMatrix();
}



void TetrisBoard::cycle() {
	int effective_speed; //convert speed (1-100) to delay value in millseconds

	//Cycle and movement delays
	if (speed > 0) {
		if (speed < 10) {
			effective_speed = 1100 + 1000 * speed;
		} else {
			effective_speed = 10000 + 10 * speed;
		}
		
		if (SDL_GetTicks() - last_cycle_time > 11100 - effective_speed) { 
			cycles = 0;
			move_active_down();
			last_cycle_time = SDL_GetTicks();
		}
	}
	//int effective_speed = pow((speed * 10000), 1.0/3.0);//Simulate linear speed progression  
	//if (cycles > 115 - effective_speed) { 
	//	cycles = 0;
	//	move_active_down();
	//}

	//Speed of 1 will correspond to 10 seconds between a cycle
	//Speed of 10 will correspond to 1 second between a cycle
	//Speed of 100 will correspond to .1 seconds between a cycle

	//Animations
	if (move_animation_delta_x > 0) {
		xPos++;
		move_animation_delta_x--;
	} else if (move_animation_delta_x < 0) {
		xPos--;
		move_animation_delta_x++;
	} else {
		move_animation_active = false;
	}
}


void TetrisBoard::rotate_active() {
	remove_active_from_grid();
	active_piece->rotate();

	if (block_collision()) {
		active_piece->rotate_back();
	} else {

		//Yeah, I know.  This is messy.
		while (right_collision()) {
			active_piece_x--;
			if (block_collision()) {
				active_piece_x++;
				active_piece->rotate_back();
				while (block_collision()) {
					active_piece_x++;
				}

				break;
			}
		}
		while (left_collision()) {
			active_piece_x++;
			if (block_collision()) {
				active_piece_x--;
				active_piece->rotate_back();
				while (block_collision()) {
					active_piece_x--;
				}
				break;
			}
		}
		while (bottom_collision()) {
			active_piece_y--;
			if (block_collision()) {
				active_piece_y++;
				active_piece->rotate_back();
				break;
			}
		}
	}
	update_grid();
}

void TetrisBoard::move_active_left() {
	remove_active_from_grid();
	active_piece_x--;
	if (left_collision() || block_collision()) {
		active_piece_x++;
	}
	update_grid();
}

void TetrisBoard::move_active_right() {
	remove_active_from_grid();
	active_piece_x++;
	if (right_collision() || block_collision()) {
		active_piece_x--;
	}
	update_grid();
}

void TetrisBoard::move_active_down() {
	bool ret = 1;
	remove_active_from_grid();
	active_piece_y++;
	if (bottom_collision() || block_collision()) {
		active_piece_y--;
		ret = 0;
	}
	update_grid();
	cycles = 0;

	//If active piece can't move down anymore
	if (!ret) {
		resolve_bottom_collision();
	}
}


void TetrisBoard::sink_active() {
	cycles = -20; //Enforce a brief pause
	remove_active_from_grid();
	do {
		active_piece_y++;
	} while (!bottom_collision() && !block_collision());

	active_piece_y--; //Bring it back up one row because of the collision
	update_grid();

	resolve_bottom_collision();
}


//Animations
void TetrisBoard::add_move_animation(int dx) {
	move_animation_active = true;
	move_animation_delta_x = dx;
}

//Accessors
bool TetrisBoard::is_dead() { return dead; }
int TetrisBoard::get_xPos() { return xPos; }
int TetrisBoard::get_yPos() { return yPos; }
TetrisPiece * TetrisBoard::get_active_piece() {return active_piece;}


//Setters
void TetrisBoard::set_board_width(int w) {board_width = w;}
void TetrisBoard::decrease_board_height() {
	remove_active_from_grid();

	//If there are any blocks in the top row, game over
	for (int i = 0; i < board_width; i++) {
		if (grid[i]) {
			dead = true;
			break;
		}
	}

	if (!dead) {
		//Shift up one row
		shift_grid_up();
		board_height--;
		if (active_piece_y > 0) {
			active_piece_y--;
		}
		update_grid();
	}
}
void TetrisBoard::set_xPos(int x) {xPos = x;}
void TetrisBoard::set_yPos(int y) {yPos = y;}
void TetrisBoard::set_speed(int spd) {speed = spd;}
void TetrisBoard::set_cycles(int c) {cycles = c;}
void TetrisBoard::set_active_piece(TetrisPiece * t) {
	remove_active_from_grid();
	if (t) {
		active_piece = t;
	} else {
		active_piece = next_piece;
		next_piece = generate_piece(); 
	}

	active_piece_x = board_width / 2 - 1;
	active_piece_y = 0;
	update_grid();
}

void TetrisBoard::set_next_piece(TetrisPiece * t) {
	delete next_piece;
	next_piece = t;
}

//Private
/* Initialize the tetris board with an empty grid */
void TetrisBoard::init_grid() {
	srand((unsigned)time(0)); //seed the random generator w/ time

	for (int i =0; i < (board_width+2)*(board_height+2); i++) {
		grid[i] = 0;
	}

	active_piece = generate_piece();
	next_piece = generate_piece();

	active_piece_x = board_width / 2 - 1;
	active_piece_y = 0;
	update_grid();
}

void TetrisBoard::draw_block(int x, int y, int square_size, int color, bool shadow) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	set_block_color(color, shadow);
	glBegin(GL_QUADS);
		glVertex2i(x*square_size+1, y*square_size-1);
		glVertex2i((x+1)*square_size-1, y*square_size-1);
		glVertex2i((x+1)*square_size-1, (y-1)*square_size+1);
		glVertex2i(x*square_size+1, (y-1)*square_size+1);
	glEnd();
}
void TetrisBoard::draw_shadow_piece(int square_size) {
	int active_piece_y_copy, row, col;
	int * active_state = active_piece->get_state();

	active_piece_y_copy = active_piece_y;
	remove_active_from_grid();

	do {
		active_piece_y++;
	} while (!bottom_collision() && !block_collision());

	active_piece_y--; //Bring it back up one row because of the collision
	
	for (int i = 0; i < 16; i++) {
		row = board_height - (active_piece_y - 2 + (i /4));
		col = active_piece_x + (i % 4);
		if (active_state[i]) {
			draw_block(col, row, square_size, active_state[i], true);
		}
	}

	active_piece_y = active_piece_y_copy;
	update_grid();
}


/* Check if the active piece has collided with the right boundary */
bool TetrisBoard::right_collision() {
	int * state = active_piece->get_state();

	//Convert state coordinates to grid coordinates then check if out of bounds
	for (int i = 0; i < 16; i++) {
		if (state[i]) {
			if ( (i % 4) + active_piece_x >= board_width) {
				return 1;
			}
		}
	}

	return 0;
}

/* Check if the active piece has collided with the left boundary */
bool TetrisBoard::left_collision() {
	int * state = active_piece->get_state();

	//Convert state coordinates to grid coordinates then check if out of bounds
	for (int i = 0; i < 16; i++) {
		if (state[i]) {
			if ( (i % 4) + active_piece_x < 0){
				return 1;
			}
		}
	}

	return 0;
}

/* Check if the active piece has collided with the bottom boundary */
bool TetrisBoard::bottom_collision() {
	int * state = active_piece->get_state();

	//Convert state coordinates to grid coordinates then check if out of bounds
	for (int i = 0; i < 16; i++) {
		if (state[i]) {
			if ( (i / 4) + active_piece_y >= board_height) {
				return 1;
			}
		}
	}

	return 0;
}


/* Check if the active piece has collided with something inside the board */
bool TetrisBoard::block_collision() {
	int * state = active_piece->get_state();

	for (int i = 0; i < 16; i++) {
		if (state[i]) {
			if (grid[(active_piece_y + i/4)*(board_width+2) + active_piece_x + (i % 4)] ) {
				return 1;
			}
		}
	}

	return 0;
}

/* Update the board so that the active piece is included in the grid */
void TetrisBoard::update_grid() {
	int * state = active_piece->get_state();

	for (int i = 0; i < 16; i++) {
		if (state[i]) {
			grid[(active_piece_y + i/4)*(board_width+2) + active_piece_x + (i %4)] = state[i];
		}
	}
}

/* Update the board so that the active piece is removed from the grid */
void TetrisBoard::remove_active_from_grid() {
	int * state = active_piece->get_state();
	for (int i = 0; i < 16; i++) {
		if (state[i]) {
			grid[(active_piece_y + i/4)*(board_width+2) + active_piece_x + (i %4)] = 0;
		}
	}
}

/* Check if the board has any lines, remove them, and update the score accordingly */
void TetrisBoard::remove_lines() {
	bool hole_found = 0;
	int row = 0;
	int last_row = 0;
	int num_lines = 0;
	

	for (int i = 0; i < 4; i++) {
		row = active_piece_y + i;
		hole_found = 0;
		for (int j = 0; j < board_width; j++) {
			if (!grid[row *(board_width+2) + j]) {
				hole_found = 1;
				break;
			}
		}

		if (!hole_found) {
		//Remove this line and bring all lines above this one down 1
			for (int i = 0; i < board_width; i++) {
				grid[row*(board_width+2) + i] = 0;
			}
			shift_grid_down(row);
			num_lines++;
			last_row = row;
		}
	}
	
	if (num_lines > 0) {
		TBM->report_line_clear(this, num_lines);
	}
}

void TetrisBoard::resolve_bottom_collision() {
	remove_lines();
	delete active_piece;
	active_piece = next_piece;
	active_piece_x = board_width / 2 - 1;
	active_piece_y = 0;
	next_piece = generate_piece();
	TBM->report_piece_drop(this);

	//If the generated piece collides with an existing block, set to game over
	if (block_collision()) {
		dead = true;
	}
	update_grid();
}

/* Shift all grid entries above 'line' down one row */
void TetrisBoard::shift_grid_down(int line) {
	for (int i = line; i > 0 ; i--) {
		for (int j = 0; j < board_width +2; j++) {
			grid[i * (board_width + 2) + j] = grid[(i-1)*(board_width + 2) + j];
		}
	}
}

/* Shift all grid entries up one row */
void TetrisBoard::shift_grid_up() {
	for (int i = 1; i < board_height ; i++) {
		for (int j = 0; j < board_width +2; j++) {
			grid[(i-1) * (board_width + 2) + j] = grid[(i)*(board_width + 2) + j];
		}
	}

	//Remove the bottom row
	for (int i = 0; i < board_width; i++) {
			grid[(board_height-1)*(board_width+2) + i] = 0;
	}
}

/* Preset color codes */
void TetrisBoard::set_block_color(int num, bool shadow) {
	float alpha = 1.0f;

	if (shadow) {
		alpha = 0.35f;
	}
	switch(num) {
		case RED:
			glColor4f (1.0f, 0.0f, 0.0f, alpha);
			break;
		case GREEN:
			glColor4f (0.0f, 1.0f, 0.0f, alpha);
			break;
		case BLUE:
			glColor4f (0.0f, 0.0f, 1.0f, alpha);
			break;
		case YELLOW:
			glColor4f (1.0f, 1.0f, 0.0f, alpha);
			break;
		case ORANGE:
			glColor4f (1.0f, 0.4f, 0.0f, alpha);
			break;
		case PURPLE:
			glColor4f (0.7f, 0.2f, 0.8f, alpha);
			break;
		case TEAL:
			glColor4f (0.0f, 1.0f, 1.0f, alpha);
			break;
		case GREY:
			glColor4f (0.5f, 0.5f, 0.5f, alpha);
			break;
		default:
			glColor4f (0.0f, 0.0f, 0.0f, 0.15f );
	}
}
/* Return a random TetrisPiece */
TetrisPiece * TetrisBoard::generate_piece() {
	int rand_int = rand() % 7; //Generate integer between 0 - 6
	TetrisPiece * piece;
    
	switch (rand_int) {
		case 0: piece = new BoxPiece(); break;
		case 1: piece = new LongPiece(); break;
		case 2: piece = new LPiece(); break;
		case 3: piece = new JPiece(); break;
		case 4: piece = new SPiece(); break;
		case 5: piece = new TwoPiece(); break;
		case 6: piece = new TPiece(); break;
	}
	return piece;
}

