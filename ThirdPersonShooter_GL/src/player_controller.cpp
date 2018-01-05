#include "GL/glew.h"
#include "SDL.h"
#include "player_controller.hpp"
#include "entity.hpp"
#include "physics_body.hpp"
#include "physics_world.hpp"
#include "glm/gtx/vector_angle.hpp"

void PlayerController::update()
{
	GLint mouseX = 0;
	GLint mouseY = 0;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	Uint32 mouse = SDL_GetMouseState(&mouseX, &mouseY);
	PhysicsBody* body = owner->get_component<PhysicsBody>();
	GLfloat forward_speed = 0.0f;
	GLfloat jump_speed = 0.0f;
	GLfloat rotation = 0.0f;

	if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		current_action = Action::Shooting;
	}
	else if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		current_action = Action::Aiming;
	}
	else
	{
		current_action = Action::None;
	}

	if (keystate[SDL_SCANCODE_W])
	{
		forward_speed = 150.0f;
		rotation = glm::angle(glm::cross(camera_transform->right, Transform::world_up_vector()), transform->forward);
	}
	else if (keystate[SDL_SCANCODE_S])
	{
		forward_speed = 150.0f;
		rotation = glm::angle(-glm::cross(camera_transform->right, Transform::world_up_vector()), transform->forward);
	}

	if (keystate[SDL_SCANCODE_A])
	{
		forward_speed = 150.0f;
		rotation = glm::angle(camera_transform->right, transform->forward);
	}
	else if (keystate[SDL_SCANCODE_D])
	{
		forward_speed = 150.0f;
		rotation = glm::angle(-camera_transform->right, transform->forward);
	}

	if (PhysicsWorld::on_ground(body))
	{
		if (forward_speed == 0.0f)
		{
			if (current_action == Action::Aiming)
			{
				rotation = glm::angle(glm::cross(camera_transform->right, Transform::world_up_vector()), transform->forward);
				owner->get_component<AnimationController>()->change_animation(3);
			}
			else if (current_action == Action::Shooting)
			{
				rotation = glm::angle(glm::cross(camera_transform->right, Transform::world_up_vector()), transform->forward);
				owner->get_component<AnimationController>()->change_animation(4);
			}
			else
			{
				owner->get_component<AnimationController>()->change_animation(0);
			}
		}
		else
		{
			owner->get_component<AnimationController>()->change_animation(1);
		}

		if (keystate[SDL_SCANCODE_SPACE])
		{
			jump_speed = 100.0f;
			body->apply_impulse(Transform::world_up_vector() * jump_speed);
			owner->get_component<AnimationController>()->change_animation(2);
		}
	}
	transform->rotate(glm::degrees(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 forward = glm::cross(transform->right, Transform::world_up_vector());
	glm::vec3 forward_velocity = forward * forward_speed;
	body->set_velocity(glm::vec3(forward_velocity.x, body->linear_velocity().y, forward_velocity.z));
}

void PlayerController::set_camera(Transform* cam_transform)
{
	if (transform == nullptr)
	{
		std::clog << "PlayerController::set_camera: cam_transform is null" << std::endl;
		return;
	}
	camera_transform = cam_transform;
}

std::string PlayerController::type_name()
{
	return "PlayerController";
}