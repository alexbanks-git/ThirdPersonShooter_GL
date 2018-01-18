#pragma once
#include <memory>
#include <string>
#include <vector>
#include "model.hpp"
#include "transform.hpp"
#include "component.hpp"
#include "player_controller.hpp"
#include "model.hpp"
#include "physics_body.hpp"
#include "animation_controller.hpp"
#include "camera.hpp"
#include "camera_controller.hpp"
#include "image.hpp"
#include "enemy_controller.hpp"
#include "bullet_controller.hpp"


class Entity
{
private:
	GLuint id;
	std::vector<std::unique_ptr<Component>> components;

public:
	Transform transform;
	Entity();

	/**
	 * @brief Adds a component to the entity.
	 * @param T the class type of the component
	 */
	template<class T> void add_component()
	{
		std::unique_ptr<T> component(new T(this));
		components.push_back(std::move(component));
	}

	/**
	 * @brief Retrieves the component at the specified index
	 * @param i the index of the component
	 * @returns the component at the specified index
	 */
	Component* get_component(GLuint i);

	/**
	 * @brief Returns a component with the specified type, if it's attached to this entity
	 * @param T the type of the component to search for
	 * @returns the component with the specified type
	 */
	template<class T> T* get_component()
	{
		T* comp = nullptr;
		for (GLuint i = 0; i < components.size(); i++)
		{
			if (comp = dynamic_cast<T*>(components[i].get()))
			{
				break;
			}
		}
		return comp;
	}

	/**
	 * @brief Retrieves all components attached to this entity
	 * @returns all components attached to this entity
	 */
	std::vector<Component*> get_components();

	/**
	 * @brief Returns the id of the entity
	 * @returns the id
	 */
	GLuint get_id();

	/**
	 * @brief Sets the id of the entity
	 * @param index the value to set the id to
	 */
	void set_id(GLuint index);
};