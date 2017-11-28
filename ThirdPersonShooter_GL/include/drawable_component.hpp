#pragma once
#include "component.hpp"

class DrawableComponent : public Component
{
public:
	/**
	 * @brief Draws the component to the screen.
	 */
	virtual void draw();

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	virtual std::string type_name();
};