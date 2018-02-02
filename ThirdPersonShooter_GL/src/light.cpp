#include "light.hpp"
#include "graphics.hpp"
#include "entity_manager.hpp"

Light::Light(Entity* entity) : DrawableComponent(entity)
{
	material = 3;
}

void Light::set_properties(GLfloat rad, GLfloat intens, glm::vec4 col)
{
	radius = rad;
	intensity = intens;
	color = col;
}

void Light::draw()
{
	std::string current = std::to_string(EntityManager::get_current_light());

	GLint position_location = glGetUniformLocation(Graphics::get_default_shader(), (std::string("lights[") + current + std::string("].position")).c_str());
	glUniform3fv(position_location, 1, glm::value_ptr(transform.position));

	GLint radius_location = glGetUniformLocation(Graphics::get_default_shader(), (std::string("lights[") + current + std::string("].radius")).c_str());
	glUniform1f(position_location, radius);

	GLint intensity_location = glGetUniformLocation(Graphics::get_default_shader(), (std::string("lights[") + current + std::string("].intensity")).c_str());
	glUniform1f(intensity_location, intensity);
	EntityManager::change_light();
}

std::string Light::type_name()
{
	return "Light";
}