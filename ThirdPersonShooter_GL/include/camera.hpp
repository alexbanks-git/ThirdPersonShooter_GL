#include "GL/glew.h"
#include "glm/glm.hpp"

class Camera
{
private:
	GLfloat near_clip;
	GLfloat far_clip;
	GLfloat field_of_view;
public:
	glm::vec3 world_up;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	/**
	 * @brief Initializes the camera's members.
	 */
	Camera(glm::vec3 pos, GLfloat field_of_view, GLfloat near, GLfloat far);
	/**
	 * @brief Translates the camera.
	 * @param x the amount to translate the camera on the x-axis
	 * @param y the amount to translate the camera on the y-axis
	 * @param z the amount to translate the camera on the z-axis
	 */
	void translate(GLfloat x, GLfloat y, GLfloat z);

	/**
	 * @brief Rotates the camera.
	 * @param pitch the amount to rotate around the x-axis
	 * @param yaw the amount to rotate around the y-axis
	 * @param roll the amount to rotate around the z-axis
	 */
	void rotate(GLfloat pitch, GLfloat yaw, GLfloat roll);

	/**
	 * @brief Retrieves the far clip plane of the camera.
	 * @returns the far clip plane
	 */
	GLfloat far_clip_plane();

	/**
	 * @brief Retrieves the near clip plane of the camera.
	 * @returns the near clip plane
	 */
	GLfloat near_clip_plane();

	/**
	* @brief Retrieves the field of view of the camera.
	* @returns the field of view
	*/
	GLfloat get_field_of_view();
};