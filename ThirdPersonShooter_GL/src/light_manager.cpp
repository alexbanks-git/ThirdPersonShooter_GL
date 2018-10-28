#include <vector>
#include <algorithm>
#include "graphics.hpp"
#include "light_manager.hpp"

static std::vector<Light*> lights;

void LightManager::render_lights()
{
	/*GLint light_loc = glGetUniformLocation(Graphics::get_default_shader(), "lights");
	glUniformMatrix4fv(light_loc, lights.size(), GL_FALSE, lights);*/
}

void LightManager::add_light(Light* light)
{
	lights.push_back(light);
}

void LightManager::remove_light(Light* light)
{
	lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
}