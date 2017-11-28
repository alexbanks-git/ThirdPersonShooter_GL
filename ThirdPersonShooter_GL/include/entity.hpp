#pragma once
#include <string>
#include <vector>
#include "model.hpp"
#include "transform.hpp"
#include "component.hpp"

class Entity
{
private:
	GLuint id;
	std::vector<Component*> components;

public:
	Transform transform;

	/**
	 * @brief Adds a component to the entity.
	 * @param type the class type of the component
	 */
	void add_component(std::string type);

	/**
	 * @brief Rerieves a component with a specific class type.
	 * @param type the class type of the component
	 */
	Component* get_component(std::string type);

	/**
	 * @brief Retrieves the component at the specified index
	 * @param i the index of the component
	 */
	Component* get_component(GLuint i);

	/**
	 * @brief Retrieves all components attached to this entity
	 * @returns all components
	 */
	std::vector<Component*> get_components();
};