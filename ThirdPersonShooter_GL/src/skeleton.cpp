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

void Skeleton::add_position_key(GLuint index, std::string name, GLfloat time, glm::vec3 pos)
{
	PositionKey position_key;
	position_key.time = time;
	position_key.position = pos;
	Bone* bone;

	if ((bone = bone_by_name(name)) != nullptr)
	{
		GLuint count = 0;
		while (index >= bone->position_keys.size())
		{
			bone->position_keys.push_back(std::vector<PositionKey>());
			if (bone->position_keys.size() - 1 != index)
			{
				bone->position_keys[bone->position_keys.size() - 1].push_back(PositionKey());
			}
		}
		bone->position_keys[index].push_back(position_key);
	}
}

void Skeleton::add_rotation_key(GLuint index, std::string name, GLfloat time, glm::quat rot)
{
	RotationKey rotation_key;
	rotation_key.time = time;
	rotation_key.rotation = rot;
	Bone* bone;
	if ((bone = bone_by_name(name)) != nullptr)
	{
		while (index >= bone->rotation_keys.size())
		{
			bone->rotation_keys.push_back(std::vector<RotationKey>());
			if (bone->rotation_keys.size() - 1 != index)
			{
				bone->rotation_keys[bone->rotation_keys.size() - 1].push_back(RotationKey());
			}
		}

		bone->rotation_keys[index].push_back(rotation_key);
	}
}

Skeleton::Bone* Skeleton::get_bone_at(GLuint i)
{
	return &bones[i];
}

Skeleton::Bone* Skeleton::get_last_bone()
{
	return &bones[bones.size() - 1];
}

Skeleton::Bone* Skeleton::get_first_bone()
{
	return &bones[0];
}

void Skeleton::calculate_bone_positions(Bone* bone)
{
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