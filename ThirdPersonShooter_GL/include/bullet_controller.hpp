#pragma once
#include "controller_component.hpp"

class BulletController : public ControllerComponent
{
private:
	glm::vec3 velocity;
	GLfloat time_to_die;
public:
	BulletController(Entity* entity);
	/**
	 * @brief Updates the position of the bullet
	 */
	void update();

	/**
	 * @brief Returns the type of the class
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Sets the velocity of the bullet
	 * @brief vel the value to set the bullet's velocity to
	 */
	void set_velocity(glm::vec3 vel);

	/**
	 * @brief Sets the ending position of the bullet
	 * @param end the ending position
	 */
	void set_endpoint(glm::vec3 end);
};