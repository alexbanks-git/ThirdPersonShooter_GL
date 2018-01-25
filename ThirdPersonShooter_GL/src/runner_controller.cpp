#include "runner_controller.hpp"

RunnerController::RunnerController(Entity* entity) : MonsterController(entity)
{
}

void RunnerController::update()
{
	glm::vec3 other = target->transform.position - transform.position;
	GLfloat dist_to_target = glm::length(other);
	PhysicsBody* body = owner.get_component<PhysicsBody>();
	GLfloat speed = 0.0f;
	transform.look_at(glm::vec3(target->transform.position.x, transform.position.y, target->transform.position.z));
	
	if (dist_to_target < 2.0f && !target_found)
	{
		target_found = true;
	}

	if (target_found)
	{
		speed = 1.0f;
		owner.get_component<AnimationController>()->play_animation(1, true);
	}
	else
	{
		owner.get_component<AnimationController>()->play_animation(0, true);
	}
	
	glm::vec3 forward_speed = transform.forward * speed;
	body->set_velocity(glm::vec3(forward_speed.x, body->linear_velocity().y, forward_speed.z));
}

void RunnerController::set_target(Entity* entity, glm::vec3 temp_offset)
{
	target = entity;
	offset = temp_offset;
}

std::string RunnerController::type_name()
{
	return "RunnerController";
}