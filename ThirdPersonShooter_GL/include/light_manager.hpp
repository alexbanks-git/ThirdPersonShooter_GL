#pragma once
#include "light.hpp"

class LightManager
{
public:
	static void render_lights();
	static void add_light(Light* light);
	static void remove_light(Light* light);
};