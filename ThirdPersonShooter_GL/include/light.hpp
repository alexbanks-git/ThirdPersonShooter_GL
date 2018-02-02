#pragma once
#include "drawable_component.hpp"

class Light : public DrawableComponent
{
public:
	GLfloat radius;
	GLfloat intensity;
	glm::vec4 color;

	Light(Entity* entity);
	std::string type_name();
	void draw();
	void set_properties(GLfloat rad, GLfloat intens, glm::vec4 col);
};