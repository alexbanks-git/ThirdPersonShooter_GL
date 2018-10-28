#include "camera_controller.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "physics_world.hpp"
#include "input_manager.hpp"

CameraController::CameraController(Entity* entity) : ControllerComponent(entity)
{

}

void CameraController::update()
{
	mouse_state = SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
	glm::vec3 target_position = target->transform.position;
	float mouse_speed = 0.1f;
	bool aiming = false;
	Sint16 right_stick_x = InputManager::get_axis(SDL_CONTROLLER_AXIS_RIGHTX);
	Sint16 right_stick_y = InputManager::get_axis(SDL_CONTROLLER_AXIS_RIGHTY);

	if ((mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) || (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)))
	{
		aiming = true;
	}

	if (aiming)
	{
		mouse_speed *= 0.5f;
	}

	if (mouse_x > mouse_speed || right_stick_x > 18000)
	{
		yaw = mouse_speed;
	}
	else if (mouse_x < -mouse_speed || right_stick_x < -18000)
	{
		yaw = -mouse_speed;
	}
	else
	{
		yaw = 0;
	}

	if (mouse_y > mouse_speed || right_stick_y > 18000)
	{
		pitch = mouse_speed;
	}
	else if (mouse_y < -mouse_speed || right_stick_y < -18000)
	{
		pitch = -mouse_speed;
	}
	else
	{
		pitch = 0;
	}

	if (aiming)
	{
		target_position += transform.right * 0.55f;
		target_position.y += vertical_offset;
		owner.get_component<Camera>()->follow(target_position, -1.2f);
	}
	else
	{
		target_position += -transform.right * 0.05f;
		target_position.y += vertical_offset + 0.2f;
		owner.get_component<Camera>()->follow(target_position, -3.5f);
	}

	owner.get_component<Camera>()->rotate_around(target_position, yaw, pitch);
}

void CameraController::set_vertical_offset(float offset)
{
	vertical_offset = offset;
}

void CameraController::set_target(Entity* target)
{
	this->target = target;
}

std::string CameraController::type_name()
{
	return "CameraController";
}