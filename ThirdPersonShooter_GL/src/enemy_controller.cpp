#include "enemy_controller.hpp"

EnemyController::EnemyController(Entity* entity) : ControllerComponent(entity)
{
}

void EnemyController::update()
{
	glm::vec3 other = target->transform.position - transform.position;
	GLfloat dist_to_target = glm::length(other);
	PhysicsBody* body = owner.get_component<PhysicsBody>();
	GLfloat speed = 0.0f;
	transform.look_at(glm::vec3(target->transform.position.x, transform.position.y, target->transform.position.z));

	if (dist_to_target < 5.0f)
	{
		owner.get_component<AnimationController>()->play_animation(0, true);
	}
	else
	{
		speed = 0.75f;
		owner.get_component<AnimationController>()->play_animation(1, true);
	}
	glm::vec3 forward_speed = transform.forward * speed;
	body->set_velocity(glm::vec3(forward_speed.x, body->linear_velocity().y, forward_speed.z));
}

void EnemyController::set_target(Entity* entity)
{
	target = entity;
}

std::string EnemyController::type_name()
{
	return "EnemyController";
}