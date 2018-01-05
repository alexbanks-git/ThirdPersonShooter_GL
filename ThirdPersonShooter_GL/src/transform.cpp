#include <iostream>
#include "transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_access.hpp"


void Transform::local_translate(glm::vec3 pos)
{
	transform_mat = glm::translate(transform_mat, pos);
	position = glm::vec3(transform_mat[3][0], transform_mat[3][1], transform_mat[3][2]);
}

void Transform::global_translate(glm::vec3 pos)
{
	transform_mat = glm::translate(glm::mat4(), pos) * transform_mat;
	position = glm::vec3(transform_mat[3][0], transform_mat[3][1], transform_mat[3][2]);
}

void Transform::rotate(GLfloat angle, glm::vec3 axis)
{
	transform_mat = glm::rotate(transform_mat, glm::radians(angle), axis);
	forward = glm::normalize(glm::rotate(forward, glm::radians(angle), axis));
	right = glm::normalize(glm::cross(world_up_vector(), forward));
	up = glm::normalize(glm::cross(forward, right));
}

glm::vec3 Transform::world_up_vector()
{
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat3 Transform::get_rotation()
{
	return glm::mat3(right, up, forward);
}

glm::vec3 Transform::get_position()
{
	return glm::vec3(transform_mat[3][0], transform_mat[3][1], transform_mat[3][2]);
}

glm::mat4 Transform::get_transformation()
{
	return transform_mat;
}

void Transform::look_at(Transform target)
{
	forward = glm::normalize(target.position - position);
	right = glm::normalize(glm::cross(world_up_vector(), forward));
	up = glm::normalize(glm::cross(forward, right));
	transform_mat = glm::lookAt(position, position + forward, world_up_vector());
}