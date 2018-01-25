#pragma once
#include "drawable_component.hpp"
#include "SDL_ttf.h"

class TextComponent : public DrawableComponent
{
private:
	std::string text;
	TTF_Font* font;
	std::string font_path;
	GLuint font_size;
	SDL_Color text_color;
	GLuint texture;
	SDL_Rect text_box;

public:
	glm::vec2 size;
	TextComponent(Entity* entity);
	void set_font_size(GLuint size);
	void set_font(std::string file_path);
	void set_text_color(SDL_Color color);
	void set_text(std::string new_text);
	void set_properties(std::string font, GLfloat size, SDL_Color color);
	void set_size(GLfloat w, GLfloat h);
	void update_texture();
	void draw();
	std::string type_name();
	static void init_ttf();
};