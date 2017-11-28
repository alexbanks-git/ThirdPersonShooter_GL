#pragma once
#include "controller_component.hpp"

class PlayerController : public ControllerComponent
{
public:
	/**
	 * @brief Handles player input 
	 */
	void update();

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	std::string type_name();
};