#pragma once
#include "controller_component.hpp"
#include "glm/glm.hpp"

class Button : public ControllerComponent
{
private:
	GLfloat width;
	GLfloat height;
	GLfloat half_width;
	GLfloat half_height;
	glm::vec4 default_color;
	glm::vec4 color;
	glm::vec4 hover_color;
	Entity* image;
	bool button_held;

public:
	Button(Entity* entity);
	void(*on_click)(Button*);

	/**
	* @brief 
	*/
	void update();

	void set_properties(glm::vec2 pos, GLfloat w, GLfloat h);

	void set_position(glm::vec2 pos);

	void set_position(GLfloat x, GLfloat y);

	void set_size(glm::vec2 size);

	void set_size(GLfloat w, GLfloat h);

	void set_color(glm::vec4 col);

	void set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
};