#include <iostream>
#include "animation.hpp"
#include "SDL.h"
#include "entity.hpp"

Animation::Animation(Skeleton* skel) : skeleton(skel)
{
	for (GLuint b = 0; b < skel->size(); b++)
	{
		position_keys.insert(std::make_pair(skel->get_bone_at(b)->name, std::vector<PositionKey>()));
		rotation_keys.insert(std::make_pair(skel->get_bone_at(b)->name, std::vector<RotationKey>()));
	}
}

void Animation::add_position_key(std::string bone_name, GLfloat time, glm::vec3 position)
{
	PositionKey key;
	key.time = time;
	key.position = position;
	position_keys[bone_name].push_back(key);
}

void Animation::add_rotation_key(std::string bone_name, GLfloat time, glm::quat rotation)
{
	RotationKey key;
	key.time = time;
	key.rotation = rotation;
	rotation_keys[bone_name].push_back(key);
}

void Animation::set_path(std::string file_path)
{
	path = file_path;
}

void Animation::set_duration(GLfloat length)
{
	duration = length;
}

void Animation::set_root_bone(std::string bone_name)
{
	root_bone = bone_name;
}

glm::vec3 Animation::get_delta_position()
{
	return delta_position;
}

void Animation::constrain_axes(glm::vec3 axes)
{
	axis_constraint = axes;
	constraints_active = true;
}

void Animation::play(bool loop)
{
	start_time = 0;
	elapsed_time = 0;
	playing = true;
	looping = loop;
}

glm::vec3 Animation::interpolate_position(PositionKey start_key, PositionKey end_key)
{
	glm::vec3 start_pos = start_key.position;
	glm::vec3 end_pos = end_key.position;
	GLfloat pos_delta_time = start_key.time - end_key.time;
	GLfloat pos_factor = (elapsed_time - start_key.time) / pos_delta_time;
	if (pos_factor > 1) pos_factor = 1;
	if (pos_factor < 0) pos_factor = 0;
	return glm::mix(start_pos, end_pos, pos_factor);
}

glm::quat Animation::interpolate_rotation(RotationKey start_key, RotationKey end_key)
{
	glm::quat start_rot = start_key.rotation;
	glm::quat end_rot = end_key.rotation;
	GLfloat rot_delta_time = end_key.time - start_key.time;
	GLfloat rot_factor = (elapsed_time - start_key.time) / rot_delta_time;
	if (rot_factor > 1) rot_factor = 1;
	if (rot_factor < 0) rot_factor = 0;
	return glm::slerp(start_rot, end_rot, rot_factor);
}

void Animation::stop()
{
	playing = false;
}

bool Animation::is_playing()
{
	return playing;
}

bool Animation::is_looping()
{
	return looping;
}

bool Animation::has_constraints()
{
	return constraints_active;
}

void Animation::update()
{
	if (!playing)
		return;

	if (start_time == 0)
	{
		start_time = SDL_GetTicks();
	}

	elapsed_time = (SDL_GetTicks() - start_time) / 30.0f;

	if (elapsed_time >= duration)
	{
		if (looping)
		{
			start_time = 0;
			elapsed_time = 0;
		}
		else
		{
			playing = false;
		}
	}

	for (GLuint b = 0; b < skeleton->size(); b++)
	{
		glm::vec3 pos_result;
		glm::quat rot_result;
		std::string bone_name = skeleton->get_bone_at(b)->name;

		for (GLuint p = 0; p < position_keys[bone_name].size(); p++)
		{
			if (p == position_keys[bone_name].size() - 1)
			{
				pos_result = position_keys[bone_name][p].position;
				break;
			}

			if (elapsed_time < position_keys[bone_name][p + 1].time)
			{
				pos_result = interpolate_position(position_keys[bone_name][p], position_keys[bone_name][p + 1]);
				break;
			}
		}

		for (GLuint r = 0; r < rotation_keys[bone_name].size(); r++)
		{
			if (r == rotation_keys[bone_name].size() - 1)
			{
				rot_result = rotation_keys[bone_name][r].rotation;
				break;
			}

			if (elapsed_time < rotation_keys[bone_name][r + 1].time)
			{
				rot_result = interpolate_rotation(rotation_keys[bone_name][r], rotation_keys[bone_name][r + 1]);
				break;
			}
		}
		if (bone_name == root_bone && constraints_active)
		{
			if (!first_frame_passed)
			{
				new_pos.x = skeleton->get_bone_at(b)->transform[3][0];
				new_pos.y = skeleton->get_bone_at(b)->transform[3][1];
				new_pos.z = skeleton->get_bone_at(b)->transform[3][2];
				first_frame_passed = true;
			}
			glm::mat4 previous_transform = glm::translate(skeleton->get_bone_at(b)->other_transform, new_pos) * glm::toMat4(rot_result);
			prev_pos.x = previous_transform[3][0];
			prev_pos.y = previous_transform[3][1];
			prev_pos.z = previous_transform[3][2];

			glm::mat4 new_transform = glm::translate(skeleton->get_bone_at(b)->other_transform, pos_result) * glm::toMat4(rot_result);
			new_pos.x = new_transform[3][0];
			new_pos.y = new_transform[3][1];
			new_pos.z = new_transform[3][2];

			delta_position = new_pos - prev_pos;
			delta_position *= axis_constraint;

			glm::vec3 constrained_transform;
			constrained_transform.x = axis_constraint.x <= 0 ? pos_result.x : skeleton->get_bone_at(b)->transform[3][0];
			constrained_transform.y = axis_constraint.y <= 0 ? pos_result.y : skeleton->get_bone_at(b)->transform[3][1];
			constrained_transform.z = axis_constraint.z <= 0 ? pos_result.z : skeleton->get_bone_at(b)->transform[3][2];

			skeleton->get_bone_at(b)->transform = glm::translate(skeleton->get_bone_at(b)->world_transform.get_transformation(), constrained_transform) * glm::toMat4(rot_result);
		}
		else
		{
			skeleton->get_bone_at(b)->transform = glm::translate(skeleton->get_bone_at(b)->world_transform.get_transformation(), pos_result) * glm::toMat4(rot_result);
		}
	}
	skeleton->bone_transforms.resize(skeleton->size());
	skeleton->calculate_bone_positions(skeleton->get_first_bone());
	for (GLuint b = 0; b < skeleton->size(); b++)
	{
		skeleton->bone_transforms[b] = skeleton->get_bone_at(b)->final_transform * skeleton->get_bone_at(b)->offset_matrix;
	}
}

void Animation::copy(Animation* anim)
{
	std::map<std::string, std::vector<Animation::PositionKey>>::iterator p_it;
	for (p_it = anim->position_keys.begin(); p_it != anim->position_keys.end(); p_it++)
	{
		position_keys[p_it->first] = p_it->second;
	}

	std::map<std::string, std::vector<Animation::RotationKey>>::iterator r_it;
	for (r_it = anim->rotation_keys.begin(); r_it != anim->rotation_keys.end(); r_it++)
	{
		rotation_keys[r_it->first] = r_it->second;
	}

	skeleton->bone_transforms = anim->skeleton->bone_transforms;
	duration = anim->duration;
}