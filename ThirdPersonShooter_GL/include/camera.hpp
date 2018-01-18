#pragma once
#include "component.hpp"

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "transform.hpp"

class Camera : public Component
{
private:
	GLfloat near_clip;
	GLfloat far_clip;
	GLfloat field_of_view;
public:
	Camera(Entity* entity);

	/**
	 * @brief Returns the far clipping plane of the camera
	 * @returns the far clipping plane
	 */
	GLfloat far_clip_plane();

	/**
	 * @brief Retrieves the near clipping plane of the camera
	 * @returns the near clipping plane
	 */
	GLfloat near_clip_plane();

	/**
	 * @brief Retrieves the field of view of the camera.
	 * @returns the field of view
	 */
	GLfloat get_field_of_view();

	/**
	 * @brief Rotates the camera around the specified position
	 * @param target the position to rotate around
	 * @param yaw the amount to rotate around the y-axis
	 * @param pitch the amount to rotate around the x-axis
	 */
	void rotate_around(glm::vec3 target, GLfloat yaw, GLfloat pitch);

	/**
	 * @brief Makes the camera follow the specified target
	 * @param target the target vector to follow
	 * @param distance the distance to follow the target from
	 */
	void follow(glm::vec3 target, GLfloat distance);

	/**
	 * @brief Initializes the field of view of the camera
	 * @param field_of_view the value to set the camera's field of view
	 */
	void set_field_of_view(GLfloat field_of_view);

	/**
	 * @brief Initializes the camera's clipping planes
	 * @param near the value to set the camera's near clipping plane
	 * @param far the value to set the camera's far clipping plane
	 */
	void set_clipping_planes(GLfloat near, GLfloat far);
};