#include <iostream>
#include "transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/quaternion.hpp"


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

void Transform::set_position(glm::vec3 pos)
{
	transform_mat[3][0] = pos.x;
	transform_mat[3][1] = pos.y;
	transform_mat[3][2] = pos.z;
	position = glm::vec3(transform_mat[3][0], transform_mat[3][1], transform_mat[3][2]);
}

void Transform::rotate(GLfloat angle, glm::vec3 axis)
{
	transform_mat = glm::rotate(transform_mat, glm::radians(angle), axis);
	forward = glm::normalize(glm::rotate(forward, glm::radians(angle), axis));
	right = glm::normalize(glm::cross(world_up_vector(), forward));
	up = glm::normalize(glm::cross(forward, right));
}

void Transform::rotate(GLfloat yaw, GLfloat pitch, GLfloat roll)
{
	rotate(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	rotate(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	rotate(roll, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::set_rotation(glm::mat3 rot)
{
	transform_mat[0][0] = right.x = rot[0][0];
	transform_mat[0][1] = right.y = rot[0][1];
	transform_mat[0][2] = right.z = rot[0][2];

	transform_mat[1][0] = up.x = rot[1][0];
	transform_mat[1][1] = up.y = rot[1][1];
	transform_mat[1][2] = up.z = rot[1][2];

	transform_mat[2][0] = forward.x = rot[2][0];
	transform_mat[2][1] = forward.y = rot[2][1];
	transform_mat[2][2] = forward.z = rot[2][2];
}

void Transform::set_direction_vectors()
{
	right.x = transform_mat[0][0];
	right.y = transform_mat[0][1];
	right.z = transform_mat[0][2];

	up.x = transform_mat[1][0];
	up.y = transform_mat[1][1];
	up.z = transform_mat[1][2];

	forward.x = transform_mat[2][0];
	forward.y = transform_mat[2][1];
	forward.z = transform_mat[2][2];
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
	return position;
}

glm::mat4 Transform::get_transformation()
{
	return transform_mat;
}

void Transform::look_at(glm::vec3 target)
{
	forward = glm::normalize(target - position);
	right = glm::normalize(glm::cross(world_up_vector(), forward));
	up = glm::normalize(glm::cross(forward, right));
	transform_mat = glm::mat4(
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		forward.x, forward.y, forward.z, 0,
		position.x, position.y, position.z, 1);
}