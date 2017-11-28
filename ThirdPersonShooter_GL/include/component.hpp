#pragma once
#include <string>
#include "transform.hpp"
class Entity;
class Component
{
protected:
	Entity* owner;
public:
	Transform* transform;
	
	/**
	 * @brief Sets the entity that this component belongs to.
	 * @param entity the owner of this component
	 */
	virtual void set_owner(Entity* entity);

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	virtual std::string type_name();
};