#include "SDL.h"
#include "spawn.hpp"
#include "level_manager.hpp"

Button::Button(Entity* entity) : ControllerComponent(entity)
{
	hover_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
}

void Button::set_position(glm::vec2 pos)
{
	transform.set_position(glm::vec3(pos, 0.0f));
}

void Button::set_position(GLfloat x, GLfloat y)
{
	transform.set_position(glm::vec3(x, y, 0.0f));
}

void Button::update()
{
	GLint mouse_x = 0;
	GLint mouse_y = 0;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	Uint32 mouse = SDL_GetMouseState(&mouse_x, &mouse_y);

	glm::vec2 top_left = glm::vec2(transform.position.x - half_width, transform.position.y - half_height);
	glm::vec2 top_right = glm::vec2(transform.position.x + half_width, transform.position.y - half_height);
	glm::vec2 bottom_left = glm::vec2(transform.position.x - half_width, transform.position.y + half_height);
	glm::vec2 bottom_right = glm::vec2(transform.position.x + half_width, transform.position.y + half_height);

	if (mouse_x > top_left.x && mouse_x < top_right.x
		&& mouse_y < bottom_right.y && mouse_y > top_left.y)
	{
		color = hover_color;
		if ((mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) && !button_held)
		{

			on_click(this);
		}
	}
	else
	{
		color = default_color;
	}

	image->get_component<Image>()->set_color(color);
	button_held = mouse & SDL_BUTTON(SDL_BUTTON_LEFT);
}

void Button::set_size(glm::vec2 size)
{
	width = size.x;
	height = size.y;
	half_width = width / 2;
	half_height = height / 2;
}

void Button::set_size(GLfloat w, GLfloat h)
{
	width = w;
	height = h;
	half_width = width / 2;
	half_height = height / 2;
}

void Button::set_properties(glm::vec2 pos, GLfloat w, GLfloat h)
{
	set_position(pos);
	set_size(w, h);
	image = &Spawn::spawn_image(glm::vec3(pos, 0.0f), w, h, "crosshair.png");
}

void Button::set_color(glm::vec4 col)
{
	default_color = col;
}

void Button::set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	default_color.r = r;
	default_color.g = g;
	default_color.b = b;
	default_color.a = a;
	image->get_component<Image>()->set_color(default_color);
}