//TextAnimation.h - A HUD element for displaying animated text
#ifndef __TextAnimation_H__
#define __TextAnimation_H__

#include <string>

struct TextAnimation {
	TextAnimation(std::string v, int x, int y, int c);
	~TextAnimation();
	 std::string value;					//value of text to be displayed
	 int xPos;							//Global X coordinate
	 int yPos;							//Global Y coordinate
	 int current_cycles;				//num cycles this animation has been active
	 int max_cycles;					//num cycles before animation ends
};

#endif //__TextAnimation_H__