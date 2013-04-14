/* -- Include the precompiled libraries -- */



#include <windows.h>

#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include "Gui.h"
#include "TetrisBoardClassicManager.h"
#include "TetrisBoardMultiManager.h"
#include "TetrisBoardSurvivalManager.h"
#include "TetrisBoardVersusManager.h"
#include "TetrisBoard.h"
#include <sstream> //for converting ints to strings
#include <fstream> //For file I/O

//#include "TGAImg.h"

#define GAME_STATE_MENU 1
#define GAME_STATE_TUTORIAL 2
#define GAME_STATE_PLAY 3
#define GAME_STATE_GAME_OVER 4

#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL ((1.0 / MAXIMUM_FRAME_RATE) * 1000)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

int game_state = GAME_STATE_MENU;
int screen_width = 800;
int screen_height = 600;
int fps = 0;
char fontPath[] = "arial.ttf"; //This font needs to be in the local directory

Gui * g = NULL;
TetrisBoardManager * p1 = NULL;

/* Draw the game screen */
static void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//f->SDL_GL_RenderText("v0.50", color, &position);

	switch (game_state) {
		case GAME_STATE_MENU:
			g->draw_main_menu();
			g->draw_string("v0.85", 0, 0, 0, 0, 255);
			break;
		case GAME_STATE_TUTORIAL:
			p1->draw_controls();
			break;
		case GAME_STATE_PLAY:
			g->draw_background();
			p1->draw();
			p1->draw_hud();
			g->draw_text_animation();
			break;
		case GAME_STATE_GAME_OVER:
			p1->draw_game_over();
			break;
	}

	SDL_GL_SwapBuffers();
}

static void init(int argc, char *argv[])
{
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	/* background color (black) */
	glClearColor( 0.0, 0.0, 0.0, 0.0 );

	/* mapping of vertices to display */
	glMatrixMode (GL_PROJECTION);
	gluOrtho2D (0.0, screen_width, 0.0, screen_height);

	glMatrixMode (GL_MODELVIEW);
}

static void generate_high_score() {
	std::ofstream myfile;
	myfile.open ("example.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
}

int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	int done;
	Uint8 *keys;
	int currentFPS = 0;
	double pastFPSTime = 0;
	static double lastFrameTime = 0.0;
	static double cyclesLeftOver = 0.0;
	double currentTime = 0.0;
	double updateIterations = 0.0;
	int cycles = 0;
	int menu_option = 0;

	SDL_Init(SDL_INIT_VIDEO);
	//SDL_EnableKeyRepeat(1, 100);  //Turn off delay when holding down a key

	screen = SDL_SetVideoMode(screen_width, screen_height, 16, SDL_OPENGL|SDL_ANYFORMAT);
	//screen = SDL_SetVideoMode(screen_width, screen_height, 16, SDL_OPENGL|SDL_FULLSCREEN);
	if ( ! screen ) {
		fprintf(stderr, "Couldn't set 800x600 GL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	SDL_WM_SetCaption("Chez-Tris?", "something");

	init(argc, argv);
	if(TTF_Init()) {
		fprintf(stderr, "Error in initializing TTF: %s\n", TTF_GetError());
		SDL_Quit();
		exit(2);
	}

	g = new Gui(fontPath, "res/space_background.tga");
	
	done = false;
	/* Main game loop */
	while ( ! done ) {
		SDL_Event event;
		currentTime = SDL_GetTicks();

		/****************************************** Beginning of States ********************************************/
		switch (game_state) {

			/****************************************** Main Menu State ********************************************/
			case GAME_STATE_MENU:
				draw();
				while ( SDL_PollEvent(&event) ) {
					switch(event.type) {
						case SDL_KEYDOWN:
							switch(event.key.keysym.sym){
								case SDLK_RETURN:
									menu_option = g->get_menu_option();
									switch(menu_option) {
										case 0:
											p1 = new TetrisBoardClassicManager(g);
											game_state = GAME_STATE_TUTORIAL;
											break;
										case 1:
											p1 = new TetrisBoardMultiManager(g);
											game_state = GAME_STATE_TUTORIAL;
											break;
										case 2:
											p1 = new TetrisBoardSurvivalManager(g);
											game_state = GAME_STATE_TUTORIAL;
											break;
										case 3:
											p1 = new TetrisBoardVersusManager(g);
											game_state = GAME_STATE_TUTORIAL;
											break;
										case 4:
											break;
										default:
											done = true;
											break;
									}
									break;
								case SDLK_UP:
									g->move_menu_up();
									break;
								case SDLK_DOWN:
									g->move_menu_down();
									break;
								case SDLK_ESCAPE:
									done = true;
									break;
							}
							break;
						case SDL_QUIT:
							done = true;
							break;
					}
				}
				break;

			/****************************************** Tutorial State ********************************************/
			case GAME_STATE_TUTORIAL:
				draw();
				while ( SDL_PollEvent(&event) ) {
					switch(event.type) {
						case SDL_KEYDOWN:
							switch(event.key.keysym.sym){
								case SDLK_RETURN:
									game_state = GAME_STATE_PLAY;
								break;
							}
							break;
						case SDL_QUIT:
							done = true;
							break;
					}
				}
				break;
			
			/****************************************** The Main Game State ********************************************/
			case GAME_STATE_PLAY:
				draw();
				updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);

				if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
					updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
				}
				
				//Update the game logic
				while (updateIterations > UPDATE_INTERVAL && (game_state != GAME_STATE_GAME_OVER) ) {
					updateIterations -= UPDATE_INTERVAL;
					p1->cycle();
					if (p1->handle_input()) {
						done = true;
					}

					//Check for game over condition
					if (p1->is_game_over()) {
						game_state = GAME_STATE_GAME_OVER;
					}

				}//end updateIteration loop
				
				cyclesLeftOver = updateIterations;
				lastFrameTime = currentTime;

				//FPS Updates
				currentFPS++;
				if ( currentTime - pastFPSTime >= 1000 )
				{
					fps = currentFPS;
					currentFPS = 0;
					pastFPSTime = currentTime;
				}
				break;

			/****************************************** Game Over State ********************************************/
			case GAME_STATE_GAME_OVER:
				draw();
				while ( SDL_PollEvent(&event) ) {
					switch(event.type) {
						case SDL_KEYDOWN:
							switch(event.key.keysym.sym){
								case SDLK_RETURN:
								case SDLK_ESCAPE:
									delete p1;
									p1 = NULL;
									game_state = GAME_STATE_MENU;
									g->clear_text_animations();
									//generate_high_score();
								break;
							}
							break;
						case SDL_QUIT:
							done = true;
							break;
					}
				}
				break;
		
		}//end switch(game_state)

		/****************************************** End of States ********************************************/

	}//end main game loop

	/* Clean up and Quit */
	delete g;
	if (p1 != NULL) {
		delete p1;
	}
	
	SDL_Quit();
	return 0;
}
