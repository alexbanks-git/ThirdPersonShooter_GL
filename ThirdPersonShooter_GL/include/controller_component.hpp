#pragma once
#include "component.hpp"

class ControllerComponent : public Component
{
public:
	virtual void update();

	/**
	* @brief Retrieves the type of the class.
	* @returns the class type
	*/
	virtual std::string type_name();
};