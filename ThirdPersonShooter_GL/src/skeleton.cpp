#include <iostream>
#include "skeleton.hpp"
#include "SDL.h"

void Skeleton::increase_size()
{
	bones.push_back(Bone());
}

void Skeleton::init()
{
	bone_transforms.resize(bones.size());
	calculate_bone_positions(&bones[0]);
	for (GLuint b = 0; b < bones.size(); b++)
	{
		bone_transforms[b] = bones[b].final_transform * bones[b].offset_matrix;
	}
}

GLuint Skeleton::size()
{
	return bones.size();
}

Skeleton::Bone* Skeleton::get_bone_at(GLuint index)
{
	return &bones[index];
}

Skeleton::Bone* Skeleton::get_last_bone()
{
	return &bones[bones.size() - 1];
}

Skeleton::Bone* Skeleton::get_first_bone()
{
	return &bones[0];
}

void Skeleton::rotate_bone(GLuint bone_index, GLfloat angle, glm::vec3 axis)
{
	glm::mat3 rot = glm::rotate(glm::mat4(), angle, axis);
	bones[bone_index].world_transform.set_rotation(rot);
}

void Skeleton::calculate_bone_positions(Bone* bone)
{
	if (bone == nullptr)
	{
		std::clog << "Skeleton::calculate_bone_positions: bone is null" << std::endl;
		return;
	}
	if (bone->parent_index != -1)
	{
		bone->final_transform = bones[bone->parent_index].final_transform * bone->transform;
	}

	for (GLuint c = 0; c < bone->children.size(); c++)
	{
		calculate_bone_positions(&bones[bone->children[c]]);
	}
}

Skeleton::Bone* Skeleton::bone_by_name(std::string name)
{
	for (GLuint b = 0; b < bones.size(); b++)
	{
		if (bones[b].name.compare(name) == 0)
		{
			return &bones[b];
		}
	}
	return nullptr;
}