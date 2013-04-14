//Gui.h - Handles the displaying of all graphical user interface elements
#ifndef __GUI_H__
#define __GUI_H__

#include "font.cpp"
#include "CTargaImage.h"
#include "TextAnimation.h"
#include <list>
#include <sstream> //for converting ints to strings
#include <iomanip> //for zero padding numerical strings

extern int screen_width;
extern int screen_height;

class Gui {
public:
	Gui(char * fontPath, char * backgroundImage);
	~Gui();

	void cycle();
	void draw_string(std::string s, int x, int y, int r, int g, int b);
	void draw_int(int i, int x, int y, int r, int g, int b);
	void draw_int(int i, int x, int y, int r, int g, int b, int width, char fill_char); //for padding
	void draw_hud(int fps, int p1Score, int p1Level, int p1Lines) const;
	void draw_text_animation();
	void draw_background() const;
	void draw_main_menu();
	void add_text_animation(std::string value, int x, int y, int duration);
	void clear_text_animations();
	void move_menu_up();
	void move_menu_down();
	void draw_multiplier(int m, int timer, int r, int g, int b, int pps);

	//Accessors
	int get_menu_option();

private:
	font * f;
	CTargaImage *m_textureOne;
	unsigned int m_textureObjectOne;

	SDL_Color color;
	SDL_Rect position;
	std::stringstream ss;

	std::list<TextAnimation *> text_animations;
	int menu_option;

	void bindTexture(char * backgroundImage);
};

#endif //__GUI_H__