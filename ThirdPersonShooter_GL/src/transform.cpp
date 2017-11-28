#include "transform.hpp"

void Transform::translate(glm::vec3 pos)
{
	position = pos;
	transform_matrix = glm::translate(glm::mat4(), position);
}

void Transform::rotate()
{

}

void Transform::resize()
{

}