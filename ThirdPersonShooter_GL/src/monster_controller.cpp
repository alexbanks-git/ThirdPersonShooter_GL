#include "monster_controller.hpp"
#include "physics_body.hpp"
#include "entity.hpp"

MonsterController::MonsterController(Entity* entity) :ControllerComponent(entity)
{

}

void MonsterController::update()
{
	glm::vec3 other = target->transform.position - transform.position;
	GLfloat dist_to_target = glm::length(other);
	PhysicsBody* body = owner.get_component<PhysicsBody>();
	GLfloat speed = 0.0f;
	transform.look_at(glm::vec3(target->transform.position.x, transform.position.y, target->transform.position.z));
	if (owner.get_component<AnimationController>()->get_active_animation() != 1)
	{
		should_attack = false;
	}

	if (dist_to_target < track_distance && !target_found)
	{
		target_found = true;
	}
	
	if (dist_to_target < 1.0f)
	{
		should_attack = true;
	}

	if (target_found)
	{
		if (should_attack)
		{
			owner.get_component<AnimationController>()->play_animation(2);
		}
		else
		{
			owner.get_component<AnimationController>()->play_animation(1, true);
			if (owner.get_component<AnimationController>()->get_active_animation() == 1)
			{
				speed = max_speed;
			}
		}
	}
	else
	{
		owner.get_component<AnimationController>()->play_animation(0, true);
	}

	glm::vec3 forward_speed = transform.forward * speed;
	body->set_velocity(glm::vec3(forward_speed.x, body->linear_velocity().y, forward_speed.z));
}

void MonsterController::set_target(Entity* entity)
{
	target = entity;
}

void MonsterController::set_max_speed(GLfloat speed)
{
	max_speed = speed;
}

void MonsterController::set_tracking_distance(GLfloat dist)
{
	track_distance = dist;
}

std::string MonsterController::type_name()
{
	return "MonsterController";
}