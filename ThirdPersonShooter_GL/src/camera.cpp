#include "camera.hpp"

Camera::Camera(glm::vec3 pos, GLfloat field_of_view, GLfloat near, GLfloat far)
{
	position = pos;
	field_of_view = field_of_view;
	near_clip = near;
	far_clip = far;

	world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	forward = glm::vec3(0.0f, 0.0f, 1.0f);
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(world_up, forward));
	up = glm::normalize(glm::cross(right, forward));
}

GLfloat Camera::near_clip_plane()
{
	return near_clip;
}

GLfloat Camera::far_clip_plane()
{
	return far_clip;
}

GLfloat Camera::get_field_of_view()
{
	return field_of_view;
}

void Camera::translate(GLfloat x, GLfloat y, GLfloat z)
{
	position.x += (x * right.x) + (z * forward.x) + (y * up.x);
	position.y += (x * right.y) + (z * forward.y) + (y * up.y);
	position.z += (x * right.z) + (z * forward.z) + (y * up.z);
}

void Camera::rotate(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	forward.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	forward.y = glm::sin(glm::radians(pitch));
	forward.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(world_up, forward));
	up = glm::normalize(glm::cross(right, forward));
}