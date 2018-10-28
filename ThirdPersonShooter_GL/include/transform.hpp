#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Transform
{
private:
	glm::mat4 transform_mat;
	void Transform::set_direction_vectors();
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 position;
	glm::vec3 scale;

	/**
	 * @brief Translates the transform by the specified amount and direction 
	 * in relation to the transform's forward direction.
	 * @param the amount and direction to translate the transform by 
	 */
	void relative_translate(glm::vec3 pos);

	/**
	 * @brief Translates the transform by the specified amount and direction.
	 * @param pos the amount and direction to translate the transform by
	 */
	void global_translate(glm::vec3 pos);

	/**
	 * @brief Moves the transform to the specified position.
	 * @param pos the position to move the transform to
	 */
	void set_position(glm::vec3 pos);

	/**
	 * @brief Rotates the transform.
	 * @param angle the angle to rotate the transform by
	 * @param axis the axis to rotate around
	 */
	void rotate(GLfloat angle, glm::vec3 axis);

	/**
	 * @brief Rotates the transform.
	 * @param yaw the amount to rotate the transform around the y-axis
	 * @param pitch the amount to rotate the transform around the x-axis
	 * @param roll the amount to rotate the transform around the z-axis
	 */
	void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll);

	/**
	 * @brief Sets the rotation of the transform.
	 * @param rot the new rotation of the transform
	 */
	void set_rotation(glm::mat3 rot);

	/**
	 * @brief Returns the world's up direction
	 * @returns the world's up direction vector
	 */
	static glm::vec3 world_up_vector();

	/**
	 * @brief Returns the transform's transformation matrix
	 * @returns the transformation matrix
	 */
	glm::mat4 get_transformation();

	/**
	 * @brief Returns the rotation of the transform.
	 * @returns the rotation of the transform
	 */
	glm::mat3 get_rotation();

	/**
	 * @brief Returns the position of the transform.
	 * @returns the position of the transform
	 */
	glm::vec3 get_position();

	/**
	 * @brief Rotates the transform forward direction to face the specified position.
	 * @param target the position for the transform to look at
	 */
	void look_at(glm::vec3 target);

	void right_look_at(glm::vec3 target);

	void multiply(glm::mat4 matrix);
};