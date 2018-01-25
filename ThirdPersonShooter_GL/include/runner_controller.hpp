#pragma once
#include "entity.hpp"
#include "monster_controller.hpp"
class RunnerController : public MonsterController
{
private:
	Entity* target;
	glm::vec3 offset;
public:
	RunnerController(Entity* entity);

	/**
	 * @brief Updates the enemy's attributes
	 */
	void update();

	/**
	 * @brief Sets the enemy's targets
	 * @param entity the entity to set the enemy's target to
	 */
	void set_target(Entity* entity, glm::vec3 temp_offset);

	/**
	* @brief Retrieves the type of the class.
	* @returns the class type
	*/
	std::string type_name();

};