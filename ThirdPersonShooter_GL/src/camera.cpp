#include <iostream>
#include "camera.hpp"

Camera::Camera(Entity* entity) : Component(entity)
{

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

void Camera::follow(glm::vec3 target, GLfloat distance)
{	
	transform.set_position(target + transform.forward * distance);
}

void Camera::rotate_around(glm::vec3 target, GLfloat yaw, GLfloat pitch)
{
	transform.global_translate(transform.right * yaw);
	transform.global_translate(transform.up * pitch);
	transform.look_at(target);
}

void Camera::set_field_of_view(GLfloat field_of_view)
{
	this->field_of_view = field_of_view;
}

void Camera::set_clipping_planes(GLfloat near, GLfloat far)
{
	near_clip = near;
	far_clip = far;
}

