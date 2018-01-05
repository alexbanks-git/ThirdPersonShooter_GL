#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Transform
{
private:
	glm::mat4 transform_mat;
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 position;
	glm::vec3 scale;
	

	/**
	 * @brief Translates the transform to the specified location.
	 * @param pos to position to translate the transform to
	 */
	void local_translate(glm::vec3 pos);

	void global_translate(glm::vec3 pos);

	/**
	 * @brief Rotates the transform by the specified angle.
	 */
	void rotate(GLfloat angle, glm::vec3 axis);

	/**
	 * @brief Retrieves the world up direction
	 * @returns the world up direction
	 */
	static glm::vec3 world_up_vector();

	glm::mat4 get_transformation();

	glm::mat3 get_rotation();

	glm::vec3 get_position();

	void look_at(Transform target);
};