#pragma once
#include "entity.hpp"
#include "controller_component.hpp"
class EnemyController : public ControllerComponent
{
private:
	Entity* target;
public:
	EnemyController(Entity* entity);

	/**
	 * @brief Updates the enemy's attributes
	 */
	void update();

	/**
	 * @brief Sets the enemy's targets
	 * @param entity the entity to set the enemy's target to
	 */
	void set_target(Entity* entity);

	/**
	* @brief Retrieves the type of the class.
	* @returns the class type
	*/
	std::string type_name();
};