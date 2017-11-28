#include "GL/glew.h"
#include "SDL.h"
#include "player_controller.hpp"
#include "entity.hpp"
#include "physics_body.hpp"

void PlayerController::update()
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	PhysicsBody* body = (PhysicsBody*)owner->get_component("PhysicsBody");
	glm::vec3 velocity = glm::vec3(); 
	/*if (keystate[SDL_SCANCODE_W])
	{
		velocity.z = 10.0f;
	}
	else if (keystate[SDL_SCANCODE_S])
	{
		velocity.z = -10.0f;
	}

	if (keystate[SDL_SCANCODE_A])
	{
		velocity.x = -10.0f;
	}
	else if (keystate[SDL_SCANCODE_D])
	{
		velocity.x = 10.0f;
	}

	if (keystate[SDL_SCANCODE_SPACE])
	{
		velocity.y = 20.0f;
	}*/

	body->move(velocity);
	body->update();
}

std::string PlayerController::type_name()
{
	return "PlayerController";
}