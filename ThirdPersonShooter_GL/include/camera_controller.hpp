#pragma once
#include "controller_component.hpp"

class CameraController : public ControllerComponent
{
private:
	Entity* target;
	GLint mouse_x;
	GLint mouse_y;
	GLfloat pitch = 0;
	GLfloat yaw = 0;
	GLint mouse_state;
	GLfloat vertical_offset;
	bool able_to_zoom;

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
	 * @param target the entity to set the camera's target to
	 */
	void set_target(Entity* target);

	void set_vertical_offset(float offset);

	/**
	* @brief Sets whether or not the camera is currently allowed to zoom
	* @param can_zoom whether or not the camera is allowed to zoom
	*/
	void set_zoom_availability(bool able_to_zoom);
};