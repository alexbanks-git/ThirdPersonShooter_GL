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

void TextComponent::init_ttf()
{
	if (TTF_Init() == -1)
	{
		std::clog << "Problem initializing TTF" << std::endl;
	}
}

void TextComponent::update_texture()
{
	font = TTF_OpenFont(font_path.c_str(), font_size);
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text_color);
	GLint internal_format;
	if (surface->format->Amask)
	{
		internal_format = GL_RGBA;
	}
	else
	{
		internal_format = GL_RGB;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, surface->w, surface->h, 0, internal_format, GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);
}

void TextComponent::set_properties(std::string font, GLfloat size, SDL_Color color)
{
	font_path = font;
	text_color = color;
	update_texture();
}

void TextComponent::set_size(GLfloat w, GLfloat h)
{
	text_box.w = w;
	text_box.y = h;
}

void TextComponent::set_font_size(GLuint size)
{
	font_size = size;
	update_texture();
}

void TextComponent::set_font(std::string file_path)
{
	font_path = file_path;
	update_texture();
}

void TextComponent::set_text_color(SDL_Color color)
{
	text_color = color;
	update_texture();
}

void TextComponent::set_text(std::string new_text)
{
	text = new_text;
	update_texture();
}

void TextComponent::draw()
{
	if (font == nullptr)
	{
		return;
	}
	text_box.x = transform.position.x;
	text_box.y = transform.position.y;
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(Graphics::get_user_interface_shader(), "sampler"), 5);
	glBindVertexArray(0);
}

std::string TextComponent::type_name()
{
	return "TextComponent";
}