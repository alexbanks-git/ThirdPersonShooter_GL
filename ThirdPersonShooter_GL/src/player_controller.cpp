#include "GL/glew.h"
#include "SDL.h"
#include "player_controller.hpp"
#include "entity.hpp"
#include "physics_body.hpp"
#include "glm/gtx/vector_angle.hpp"

void PlayerController::update()
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	PhysicsBody* body = (PhysicsBody*)owner->get_component("PhysicsBody");
	glm::vec3 velocity = glm::vec3();
	GLfloat rotation = 0.0f;

	if (keystate[SDL_SCANCODE_W])
	{
		velocity.z = 5.0f;
		rotation = glm::angle(glm::cross(camera_transform->right, Transform::world_up_vector()), transform->forward);
		((AnimationController*)owner->get_component("AnimationController"))->change_animation(1);
	}
	else if (keystate[SDL_SCANCODE_S])
	{ 
		velocity.z = 5.0f;
		rotation = glm::angle(-glm::cross(camera_transform->right, Transform::world_up_vector()), transform->forward);
		((AnimationController*)owner->get_component("AnimationController"))->change_animation(1);
	}

	if (keystate[SDL_SCANCODE_A])
	{
		velocity.z = 5.0f;
		rotation = glm::angle(camera_transform->right, transform->forward);
		((AnimationController*)owner->get_component("AnimationController"))->change_animation(1);
	}
	else if (keystate[SDL_SCANCODE_D])
	{
		velocity.z = 5.0f;
		rotation = glm::angle(-camera_transform->right, transform->forward);
		((AnimationController*)owner->get_component("AnimationController"))->change_animation(1);
	}

	if (keystate[SDL_SCANCODE_SPACE])
	{
		velocity.y = 150.0f;
	}

	if (velocity.z == 0.0f)
	{
		((AnimationController*)owner->get_component("AnimationController"))->change_animation(0);
	}

	transform->rotate(glm::degrees(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	body->move(velocity);
	body->update();
}

void PlayerController::set_camera(Transform* cam_transform)
{
	camera_transform = cam_transform;
}

std::string PlayerController::type_name()
{
	return "PlayerController";
}