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
		if (index >= bone->position_keys.size())
		{
			bone->position_keys.push_back(std::vector<PositionKey>());
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
		if (index >= bone->rotation_keys.size())
		{
			bone->rotation_keys.push_back(std::vector<RotationKey>());
		}

		bone->rotation_keys[index].push_back(rotation_key);
	}
}

void Skeleton::update()
{

	if (start_time == 0)
	{
		start_time = SDL_GetTicks();
	}

	elapsed_animation_time = (SDL_GetTicks() - start_time) / 50.0f;

	if (elapsed_animation_time > durations[current_animation])
	{
		start_time = 0;
		elapsed_animation_time = 0;
	}

	for (GLuint i = 0; i < bones.size(); i++)
	{
		if (bones[i].position_keys.size() == 0)
			continue;

		glm::vec3 start_pos;
		glm::vec3 end_pos;
		glm::quat start_rot;
		glm::quat end_rot;
		glm::vec3 pos_result;
		glm::quat rot_result;
		GLfloat rot_factor = 0;
		GLfloat pos_factor = 0;
		GLfloat rot_delta_time;
		GLfloat pos_delta_time;

		for (GLuint p = 0; p < bones[i].position_keys[current_animation].size(); p++)
		{
			
			if (p == bones[i].position_keys[current_animation].size() - 1)
			{
				pos_result = bones[i].position_keys[current_animation][p].position;
				break;
			}
			
			if (elapsed_animation_time < bones[i].position_keys[current_animation][p + 1].time)
			{
				start_pos = bones[i].position_keys[current_animation][p].position;
				end_pos = bones[i].position_keys[current_animation][p + 1].position;
				pos_delta_time = bones[i].position_keys[current_animation][p + 1].time - bones[i].position_keys[current_animation][p].time;
				pos_factor = (elapsed_animation_time - bones[i].position_keys[current_animation][p].time) / pos_delta_time;
				if (pos_factor > 1) pos_factor = 1;
				if (pos_factor < 0) pos_factor = 0;
				pos_result = glm::mix(start_pos, end_pos, pos_factor);
				break;
			}
		}
		
		for (GLuint r = 0; r < bones[i].rotation_keys[current_animation].size(); r++)
		{
			if (r == bones[i].rotation_keys[current_animation].size() - 1)
			{
				rot_result = bones[i].rotation_keys[current_animation][r].rotation;
				break;
			}
			
			if (elapsed_animation_time < bones[i].rotation_keys[current_animation][r + 1].time)
			{
				
				start_rot = bones[i].rotation_keys[current_animation][r].rotation;
				end_rot = bones[i].rotation_keys[current_animation][r + 1].rotation;
				
				rot_delta_time = bones[i].rotation_keys[current_animation][r + 1].time - bones[i].rotation_keys[current_animation][r].time;
				rot_factor = (elapsed_animation_time - bones[i].rotation_keys[current_animation][r].time) / rot_delta_time;
				if (rot_factor > 1) rot_factor = 1;
				if (rot_factor < 0) rot_factor = 0;
				rot_result = glm::slerp(start_rot, end_rot, rot_factor);
				break;
			}
			
		}
		bones[i].transform = glm::translate(glm::mat4(), pos_result) * glm::toMat4(rot_result) * glm::mat4();
	}

	bone_transforms.resize(bones.size());
	calculate_bone_positions(&bones[0]);
	for (GLuint b = 0; b < bones.size(); b++)
	{
		bone_transforms[b] = bones[b].final_transform * bones[b].offset_matrix;
	}
}

void Skeleton::set_max_time(GLfloat time)
{
	durations.push_back(time);
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