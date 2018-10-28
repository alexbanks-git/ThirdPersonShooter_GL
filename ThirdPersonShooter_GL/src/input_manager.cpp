#include "input_manager.hpp"
#include "iostream"

static SDL_GameController* controller;

void InputManager::init()
{
	controller = SDL_GameControllerOpen(0);
}

bool InputManager::button_pressed(SDL_GameControllerButton button)
{
	return SDL_GameControllerGetButton(controller, button);
}

Sint16 InputManager::get_axis(SDL_GameControllerAxis axis)
{
	return SDL_GameControllerGetAxis(controller, axis);
}