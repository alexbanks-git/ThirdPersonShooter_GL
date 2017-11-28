#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Transform
{
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 transform_matrix;

	/**
	 * @brief Translates the transform to the specified location.
	 * @param pos to position to translate the transform to
	 */
	void translate(glm::vec3 pos);

	/**
	 * @brief Rotates the transform by the specified angle.
	 */
	void rotate();

	/**
	 * @brief Scales the transform by the specified amount.
	 */
	void resize();
};