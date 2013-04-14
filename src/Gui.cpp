#include "Gui.h"

/* Update anything that needs to be updated every frame */

Gui::Gui(char * fontPath, char * backgroundImage):
	 menu_option(0)
{
	font::init();
	//f = new font(fontPath);
	f = new font("res/arial.ttf");

	//Texture Stuff
	bindTexture(backgroundImage);
}
Gui::~Gui() {
	delete f;
	delete m_textureOne;
}

void Gui::cycle() {
	//Score Animations
	std::list<TextAnimation *>::iterator itr = text_animations.begin();
	while (itr != text_animations.end()) {
		(*itr)->yPos++;
		(*itr)->current_cycles++;
		if ((*itr)->current_cycles >= (*itr)->max_cycles) {
			itr = text_animations.erase(itr);
		} else {
			itr++;
		}
	}
}

/** A quick note about position.
 * Enable2D puts the origin in the lower-left corner of the
 * screen, which is different from the normal coordinate
 * space of most 2D api's. position, therefore,
 * gives the X,Y coordinates of the lower-left corner of the
 * rectangle **/
void Gui::draw_string(std::string s, int x, int y, int r, int g, int b) {
	position.x = x;
	position.y = y;
	color.r = r;
	color.g = g;
	color.b = b;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	f->SDL_GL_RenderText(s.c_str(), color, &position);
}

void Gui::draw_int(int i, int x, int y, int r, int g, int b) {
	position.x = x;
	position.y = y;
	color.r = r;
	color.g = g;
	color.b = b;
	std::string text;
	ss << i;
	ss >> text;
	ss.clear();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	f->SDL_GL_RenderText(text.c_str(), color, &position);
}

void Gui::draw_int(int i, int x, int y, int r, int g, int b, int width, char fill_char) {
	position.x = x;
	position.y = y;
	color.r = r;
	color.g = g;
	color.b = b;
	std::string text;
	ss <<  std::setw(width) << std::setfill(fill_char) << i;
	ss >> text;
	ss.clear();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	f->SDL_GL_RenderText(text.c_str(), color, &position);
}

void Gui::draw_text_animation() {
	std::list<TextAnimation *>::iterator itr;
	for (itr = text_animations.begin(); itr != text_animations.end(); itr++) {
		draw_string((*itr)->value, (*itr)->xPos, (*itr)->yPos, 255, 0, 0);
	}
}

void Gui::clear_text_animations() {
	std::list<TextAnimation *>::iterator itr = text_animations.begin();
	while (itr != text_animations.end()) {
			itr = text_animations.erase(itr);
	}

}

/* Draws the background image */
void Gui::draw_background() const {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f,1.0f,1.0f,1.0f); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, m_textureObjectOne);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);	glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex2f(screen_width, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex2f(screen_width, screen_height);
		glTexCoord2f(0.0, 1.0); glVertex2f(0.0, screen_height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Gui::draw_main_menu() {
	draw_string("Chez-Tris?!", 300, 550, 255, 255 ,255);
	draw_string("Classic", 300, 350, 255, 255 ,255);
	draw_string("Multi", 300, 300, 255, 255 ,255);
	draw_string("Survival", 300, 250, 255, 255 ,255);
	draw_string("Versus", 300, 200, 255, 255 ,255);
	draw_string("High Scores", 300, 150, 255, 255 ,255);
	draw_string("Exit", 300, 100, 255, 255 ,255);


	glPushMatrix();
		glTranslatef(250, 350-(50*menu_option), 0);
		glColor4f(0.0, 0.0, 1.0, 0.25f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);				
			glVertex2f(1, 50);
			glVertex2f(200, 50);
			glVertex2f(200, 1);
			glVertex2f(1, 1);
		glEnd();		
	glPopMatrix();
}

void Gui::draw_multiplier(int m, int timer, int r, int g, int b, int pps) {
	SDL_Color color;
	SDL_Rect position;

	std::stringstream ss;
	std::string multiplier;
	ss << m;
	ss >> multiplier;
	ss.clear();

	color.r = 255;
	color.g = 255;
	color.b = 255;



	position.x = 140;
	position.y = 500;
	f->SDL_GL_RenderText("x", color, &position);

	position.x = 150;
	f->SDL_GL_RenderText(multiplier.c_str(), color, &position);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(r, g, b, 0.25f);
	/*
	for (int i = timer; i > 0; i--) {
		glPushMatrix();
		glTranslatef(350 + i*20, 550, 0);
		glBegin(GL_QUADS);				
			glVertex2f(1, 10);
			glVertex2f(20, 10);
			glVertex2f(20, 1);
			glVertex2f(1, 1);
		glEnd();	
		glPopMatrix();
	}
	*/
	
	glPushMatrix();
		glTranslatef(150, 540, 0);
		glBegin(GL_QUADS);				
			glVertex2f(1, 10);
			//glVertex2f(timer/40, 10);
			//glVertex2f(timer/40, 1);
			glVertex2f(timer * pps / 1000, 10);
			glVertex2f(timer * pps / 1000, 1);
			glVertex2f(1, 1);
		glEnd();	
	glPopMatrix();


}

void Gui::add_text_animation(std::string value, int x, int y, int duration) {
	text_animations.push_back(new TextAnimation(value, x, y, duration));
}

void Gui::move_menu_up() {
	menu_option--;
	if (menu_option < 0){
		menu_option = 5;
	}
}

void Gui::move_menu_down() {
	menu_option++;
	if (menu_option > 5){
		menu_option = 0;
	}
}

//accessors
int Gui::get_menu_option() {
	return menu_option;
}

//private

void Gui::bindTexture(char * backgroundImage) {

	m_textureOne = new CTargaImage;
	
	// load texture image data
	if (!m_textureOne->Load(backgroundImage))
		fprintf(stderr, "Unable to load targa image");

	// retrieve "unused" texture object
	glGenTextures(1, &m_textureObjectOne);

	// bind the texture object
	glBindTexture(GL_TEXTURE_2D, m_textureObjectOne);

	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// now that the texture object is bound, specify a texture for it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureOne->GetWidth(), m_textureOne->GetHeight(),
	0, GL_RGB, GL_UNSIGNED_BYTE, m_textureOne->GetImage());
}