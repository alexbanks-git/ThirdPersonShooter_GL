#include <iostream>
#include "graphics.hpp"
#include "text_component.hpp"

static SDL_Color default_color = {255, 255, 255, 255};
static std::string default_font_path = "coolvetica rg.ttf";
static GLuint default_font_size = 36;
static SDL_Rect default_text_box = {0, 0, 150, 50};

TextComponent::TextComponent(Entity* entity) : DrawableComponent(entity)
{
	material = 1;
	text = "Place Holder";
	text_color = default_color;
	font_path = default_font_path;
	font_size = default_font_size;
	text_box = default_text_box;
}

void TextComponent::set_properties(std::string font, GLfloat size, SDL_Color color)
{
	font_path = font;
	text_color = color;
}

void TextComponent::set_size(GLfloat w, GLfloat h)
{
	text_box.w = w;
	text_box.y = h;
}

void TextComponent::set_font_size(GLuint size)
{
	font_size = size;
}

void TextComponent::set_font(std::string file_path)
{
	font_path = file_path;
}

void TextComponent::set_text_color(SDL_Color color)
{
	text_color = color;
}

void TextComponent::set_text(std::string new_text)
{
	
}

void TextComponent::draw()
{

}

std::string TextComponent::type_name()
{
	return "TextComponent";
}