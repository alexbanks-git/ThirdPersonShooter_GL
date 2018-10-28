#include "light.hpp"
#include "graphics.hpp"
#include "entity_manager.hpp"
#include "level_manager.hpp"

static float att_constant = 1.0f;
static float att_linear = 0.5f;
static float att_quadratic = 0.03f;

Light::Light(Entity* entity) : DrawableComponent(entity)
{
	material = 3;
}

void Light::set_properties(GLfloat rad, GLfloat intens, glm::vec4 col)
{
	radius = rad;
	intensity = intens;
	color = col;
	light_type = LightType::Point;
}

void Light::set_properties(glm::vec3 dir, GLfloat intens, glm::vec4 col)
{
	direction = dir;
	intensity = intens;
	color = col;
	light_type = LightType::Directional;
}

void Light::create_depth_map(GLuint width, GLuint height)
{
	dm_width = width;
	dm_height = height;
	glGenTextures(1, &depth_map);
	glBindTexture(GL_TEXTURE_2D, depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Light::create_frame_buffer_object()
{
	glGenFramebuffers(1, &frame_buffer_object);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::draw()
{
	std::string current = std::to_string(EntityManager::get_current_light());

	if (light_type == LightType::Point)
	{
		GLint position_location = glGetUniformLocation(Shader::get_current_shader(), (std::string("point_lights[") + current + std::string("].position")).c_str());
		glUniform3fv(position_location, 1, glm::value_ptr(transform.position));

		GLint radius_location = glGetUniformLocation(Shader::get_current_shader(), (std::string("point_lights[") + current + std::string("].radius")).c_str());
		glUniform1f(position_location, radius);

		GLint intensity_location = glGetUniformLocation(Shader::get_current_shader(), (std::string("point_lights[") + current + std::string("].intensity")).c_str());
		glUniform1f(intensity_location, intensity);

		GLint att_constant_location = glGetUniformLocation(Shader::get_current_shader(), (std::string("point_lights[") + current + std::string("].att_constant")).c_str());
		glUniform1f(att_constant_location, att_constant);

		GLint att_linear_location = glGetUniformLocation(Shader::get_current_shader(), (std::string("point_lights[") + current + std::string("].att_linear")).c_str());
		glUniform1f(att_linear_location, att_linear);

		GLint att_quadratic_location = glGetUniformLocation(Shader::get_current_shader(), (std::string("point_lights[") + current + std::string("].att_quadratic")).c_str());
		glUniform1f(att_quadratic_location, att_quadratic);
	}
	else if (light_type == LightType::Directional)
	{
		GLint direction_location = glGetUniformLocation(Shader::get_current_shader(), std::string("directional_light.direction").c_str());
		glUniform3fv(direction_location, 1, glm::value_ptr(direction));

		GLint intensity_location = glGetUniformLocation(Shader::get_current_shader(), std::string("directional_light.intensity").c_str());
		glUniform1f(intensity_location, intensity);
	}
	EntityManager::change_light();
}

void Light::create_shadows()
{
	float near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 light_projection;
	glm::mat4 light_view;
	glm::mat4 light_space_matrix;

	light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	light_view = glm::lookAt(transform.position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		Transform::world_up_vector());
	light_space_matrix = light_projection * light_view;

	GLint light_uniform = glGetUniformLocation(Shader::get_current_shader(), "light_matrix");
	glUniformMatrix4fv(light_uniform, 1, GL_FALSE, glm::value_ptr(light_space_matrix));
	glViewport(0, 0, dm_width, dm_height);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object);
	glClear(GL_DEPTH_BUFFER_BIT);

	LevelManager::draw_level();
}

void Light::apply_depth_map()
{
	float near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 light_view = glm::lookAt(transform.position,
		transform.position + direction,
		Transform::world_up_vector());
	glm::mat4 light_space_matrix = light_projection * light_view;

	GLint light_uniform = glGetUniformLocation(Shader::get_current_shader(), "light_matrix");
	glUniformMatrix4fv(light_uniform, 1, GL_FALSE, glm::value_ptr(light_space_matrix));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depth_map);
	glUniform1i(glGetUniformLocation(Shader::get_current_shader(), "depth_map"), 3);
}

bool Light::is_directional_light()
{
	return light_type == LightType::Directional;
}

bool Light::is_point_light()
{
	return light_type == LightType::Point;
}

std::string Light::type_name()
{
	return "Light";
}