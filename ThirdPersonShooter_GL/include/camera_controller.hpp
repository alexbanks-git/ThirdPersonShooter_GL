#pragma once
#include "controller_component.hpp"

class CameraController : public ControllerComponent
{
private:
	Transform* target;
	GLint mouse_x;
	GLint mouse_y;
	GLfloat pitch = 0;
	GLfloat yaw = 0;
	GLint mouse_state;
public:
	CameraController(Entity* entity);
	/**
	 * @brief Updates the camera's attributes
	 */
	void update();

	/**
	* @brief Retrieves the type of the class.
	* @returns the class type
	*/
	std::string type_name();

	/**
	 * @brief Sets the target of the camera
	 * @param target the transform to set the camera's target to
	 */
	void set_target(Transform* target);
};