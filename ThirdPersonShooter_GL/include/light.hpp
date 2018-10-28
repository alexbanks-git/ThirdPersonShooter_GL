#pragma once
#include "drawable_component.hpp"

class Light : public DrawableComponent
{
private:
	enum class LightType { Point, Spot, Directional };
	LightType light_type = LightType::Directional;
	GLuint depth_map;
	GLuint frame_buffer_object;
	GLuint dm_width;
	GLuint dm_height;

public:
	GLfloat radius;
	GLfloat intensity;
	glm::vec4 color;
	glm::vec3 direction;

	Light(Entity* entity);
	std::string type_name();
	void draw();
	void set_properties(GLfloat rad, GLfloat intens, glm::vec4 col);
	void set_properties(glm::vec3 dir, GLfloat intens, glm::vec4 col);
	bool is_directional_light();
	bool is_point_light();
	void create_depth_map(GLuint width, GLuint height);
	void create_frame_buffer_object();
	void create_shadows();
	void apply_depth_map();
};