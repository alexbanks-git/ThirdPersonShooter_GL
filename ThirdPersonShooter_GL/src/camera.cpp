#include <iostream>
#include "camera.hpp"

Camera::Camera(glm::vec3 pos, GLfloat field_of_view, GLfloat near, GLfloat far)
{
	transform.local_translate(pos);
	field_of_view = field_of_view;
	near_clip = near;
	far_clip = far;

	transform.forward = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
	transform.right = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
	transform.up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
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
	glm::mat3 rot = glm::mat3(transform.right, transform.up, transform.forward);
	transform.local_translate(rot * glm::vec3(x, y, z));
}

void Camera::rotate(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	transform.rotate(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	transform.rotate(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	transform.rotate(roll, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::look_at(Transform target)
{
	transform.forward = glm::normalize(target.position - transform.position);
	transform.right = glm::normalize(glm::cross(Transform::world_up_vector(), transform.forward));
	transform.up = glm::normalize(glm::cross(transform.forward, transform.right));
}

void Camera::follow(Transform target, GLfloat offset)
{
	glm::vec3 delta = (target.position + transform.forward * offset) - (transform.position);
	transform.local_translate(delta);
}

void Camera::rotate_around(Transform target, GLfloat yaw, GLfloat pitch)
{
	translate(yaw, pitch, 0.0f);
	look_at(target);
}

