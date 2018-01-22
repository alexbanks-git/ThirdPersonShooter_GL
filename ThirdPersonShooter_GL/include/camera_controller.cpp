#include "camera_controller.hpp"
#include "camera.hpp"
#include "entity.hpp"

CameraController::CameraController(Entity* entity) : ControllerComponent(entity)
{

}

void CameraController::update()
{
	mouse_state = SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
	glm::vec3 target_position = target->position;

	if (mouse_x > 0.04)
		yaw = 0.04f;
	else if (mouse_x < -0.04)
		yaw = -0.04f;
	else
		yaw = 0;

	if (mouse_y > 0.04)
		pitch = 0.04f;
	else if (mouse_y < -0.04)
		pitch = -0.04f;
	else
		pitch = 0;
	
	
	if ((mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) || (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)))
	{
		target_position += transform.right * 0.55f;
		target_position.y += 2.0f;
		owner.get_component<Camera>()->follow(target_position, -1.2f);
	}
	else
	{
		target_position += -transform.right * 0.05f;
		target_position.y += 2.2f;
		owner.get_component<Camera>()->follow(target_position, -3.5f);
	}

	owner.get_component<Camera>()->rotate_around(target_position, yaw, pitch);
}

void CameraController::set_target(Transform* target)
{
	this->target = target;
}

std::string CameraController::type_name()
{
	return "CameraController";
}