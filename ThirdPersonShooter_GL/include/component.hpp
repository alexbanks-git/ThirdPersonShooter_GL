#pragma once
#include <string>
#include "transform.hpp"
class Entity;
class Component
{
protected:
	Entity& owner;
public:
	Transform& transform;
	Component(Entity* entity);

	/**
	 * @brief Returns a reference to the component's owner entity
	 * @returns a reference to the owner entity
	 */
	virtual Entity& get_owner();

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	virtual std::string type_name();
};