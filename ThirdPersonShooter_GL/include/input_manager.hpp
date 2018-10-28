#pragma once
#include "SDL.h"

class InputManager
{
public:
	static void init();
	static bool button_pressed(SDL_GameControllerButton button);
	static Sint16 get_axis(SDL_GameControllerAxis axis);
};