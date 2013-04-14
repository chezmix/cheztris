#include "TextAnimation.h"

TextAnimation::TextAnimation(std::string v, int x, int y, int c):
	 value(v),
	 xPos(x),				
	 yPos(y),
	 current_cycles(0),
	 max_cycles(c)
{
}
TextAnimation::~TextAnimation() {}