#pragma once
#include "controller_component.hpp"

class PlayerWeapon: public ControllerComponent
{
private:
	Entity* parent;
	glm::mat4* offset;
	glm::vec3 offset_position;
	glm::mat3 offset_rotation;
	glm::vec3 local_position;
public:
	PlayerWeapon(Entity* entity);

	/**
	 * @brief Called every frame
	 */
	void update();

	/**
	 * @brief Returns the type of the class as a string
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Attaches the weapon to a specfic entity 
	 * @param entity the entity to attach to
	 * @param trans the offset from the entity's origin
	 */
	void attach_to_entity(Entity* entity, glm::mat4* trans);

	/** 
	 * @brief Retrieves the offset position and rotation
	 */
	void recalculate_offsets();
};