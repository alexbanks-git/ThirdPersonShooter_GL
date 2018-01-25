#pragma once
#include "controller_component.hpp"

class MonsterController : public ControllerComponent
{
private:
	Entity* target;
	bool target_found;
	GLfloat track_distance;
	GLfloat max_speed;
public:
	MonsterController(Entity* entity);
	void update();
	void set_target(Entity* entity);
	void set_max_speed(GLfloat speed);
	void set_tracking_distance(GLfloat dist);
	std::string type_name();
};