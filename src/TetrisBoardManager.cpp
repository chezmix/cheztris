#include "TetrisBoardManager.h"
#include "TetrisPiece.h"

#include <windows.h>
#include <GL/gl.h>

TetrisBoardManager::TetrisBoardManager():  game_over(false)  {
}

void TetrisBoardManager::draw_hold_box(TetrisPiece * hold_piece, bool hold_used, int square_size, int x, int y) {
	int * hold_state;
	if (hold_piece) {
		hold_state = hold_piece->get_state();
	}
	glBlendFunc(GL_ONE, GL_ZERO);
	//Hold Box
	glPushMatrix();
		glTranslatef(x,y,0);

		//Hold Box Cover
		glColor4f(0.0, 0.0, 0.0, 0.75f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);						
			glVertex2f(1, 4 * square_size - 1);
			glVertex2f(4 * square_size - 1, 4 * square_size - 1);
			glVertex2f(4 * square_size - 1, 1);
			glVertex2f(1, 1);
		glEnd();

		//Hold Box Outline
		if (hold_used) {
			glColor4f(0.6, 0.6, 0.6, 0.5f);
		} else {
			glColor4f(1.0, 1.0, 1.0, 0.5f);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);						
			glVertex2f(0, 4 * square_size);
			glVertex2f(4 * square_size, 4 * square_size);
			glVertex2f(4 * square_size, 0);
			glVertex2f(0, 0);
		glEnd();

		int row, col;
		//Hold Box Contents
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (hold_piece) {
			for (int i = 0; i < 16; i++) {
				if (hold_state[i]) {
					row = 4 - (i /4);
					col = i % 4;
					switch(hold_state[i]) {
						case RED:
							glColor4f (1.0f, 0.0f, 0.0f, 1.0);
							break;
						case GREEN:
							glColor4f (0.0f, 1.0f, 0.0f, 1.0);
							break;
						case BLUE:
							glColor4f (0.0f, 0.0f, 1.0f, 1.0);
							break;
						case YELLOW:
							glColor4f (1.0f, 1.0f, 0.0f, 1.0);
							break;
						case ORANGE:
							glColor4f (1.0f, 0.4f, 0.0f, 1.0);
							break;
						case PURPLE:
							glColor4f (0.7f, 0.2f, 0.8f, 1.0);
							break;
						case TEAL:
							glColor4f (0.0f, 1.0f, 1.0f, 1.0);
							break;
						case GREY:
							glColor4f (0.5f, 0.5f, 0.5f, 1.0);
							break;
						default:
							glColor4f (0.0f, 0.0f, 0.0f, 0.15f );
					}
					
					glBegin(GL_QUADS);
						glVertex2i(col*square_size+1, row*square_size-1);
						glVertex2i((col+1)*square_size-1, row*square_size-1);
						glVertex2i((col+1)*square_size-1, (row-1)*square_size+1);
						glVertex2i(col*square_size+1, (row-1)*square_size+1);
					glEnd();
				}
			}
		}
	glPopMatrix();
}

//Accessors
bool TetrisBoardManager::is_game_over() const { return game_over; }