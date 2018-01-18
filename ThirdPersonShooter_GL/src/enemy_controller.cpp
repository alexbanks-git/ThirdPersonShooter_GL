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
	transform.look_at(target->transform.position);

	if (dist_to_target < 5.0f)
	{
		owner.get_component<AnimationController>()->get_skeleton()->rotate_bone(1, -0.7f, glm::vec3(0, 1, 0));
		owner.get_component<AnimationController>()->change_animation(0);
	}
	else
	{
		speed = 0.75f;
		owner.get_component<AnimationController>()->get_skeleton()->rotate_bone(1, 0, glm::vec3(0, 1, 0));
		owner.get_component<AnimationController>()->change_animation(1);
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