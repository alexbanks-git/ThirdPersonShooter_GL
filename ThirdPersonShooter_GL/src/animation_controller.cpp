#include <iostream>
#include "animation_controller.hpp"
#include "SDL.h"

void AnimationController::update()
{
	if (start_time == 0)
	{
		start_time = SDL_GetTicks();
	}

	elapsed_time = (SDL_GetTicks() - start_time) / 50.0f;

	if (elapsed_time > durations[current_animation])
	{
		start_time = 0;
		elapsed_time = 0;
	}

	for (GLuint i = 0; i < skeleton->size(); i++)
	{
		if (skeleton->get_bone_at(i)->position_keys.size() == 0)
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
		//std::cout << skeleton->get_bone_at(i)->position_keys.size() << " | " <<current_animation<<  std::endl;
		if (skeleton->get_bone_at(i)->position_keys.size() <= current_animation) continue;
		for (GLuint p = 0; p < skeleton->get_bone_at(i)->position_keys[current_animation].size(); p++)
		{
			if (p == skeleton->get_bone_at(i)->position_keys[current_animation].size() - 1)
			{
				pos_result = skeleton->get_bone_at(i)->position_keys[current_animation][p].position;
				break;
			}

			if (elapsed_time < skeleton->get_bone_at(i)->position_keys[current_animation][p + 1].time)
			{
				start_pos = skeleton->get_bone_at(i)->position_keys[current_animation][p].position;
				end_pos = skeleton->get_bone_at(i)->position_keys[current_animation][p + 1].position;
				pos_delta_time = skeleton->get_bone_at(i)->position_keys[current_animation][p + 1].time - skeleton->get_bone_at(i)->position_keys[current_animation][p].time;
				pos_factor = (elapsed_time - skeleton->get_bone_at(i)->position_keys[current_animation][p].time) / pos_delta_time;
				if (pos_factor > 1) pos_factor = 1;
				if (pos_factor < 0) pos_factor = 0;
				pos_result = glm::mix(start_pos, end_pos, pos_factor);
				break;
			}
		}

		for (GLuint r = 0; r < skeleton->get_bone_at(i)->rotation_keys[current_animation].size(); r++)
		{
			if (r == skeleton->get_bone_at(i)->rotation_keys[current_animation].size() - 1)
			{
				rot_result = skeleton->get_bone_at(i)->rotation_keys[current_animation][r].rotation;
				break;
			}

			if (elapsed_time < skeleton->get_bone_at(i)->rotation_keys[current_animation][r + 1].time)
			{

				start_rot = skeleton->get_bone_at(i)->rotation_keys[current_animation][r].rotation;
				end_rot = skeleton->get_bone_at(i)->rotation_keys[current_animation][r + 1].rotation;

				rot_delta_time = skeleton->get_bone_at(i)->rotation_keys[current_animation][r + 1].time - skeleton->get_bone_at(i)->rotation_keys[current_animation][r].time;
				rot_factor = (elapsed_time - skeleton->get_bone_at(i)->rotation_keys[current_animation][r].time) / rot_delta_time;
				if (rot_factor > 1) rot_factor = 1;
				if (rot_factor < 0) rot_factor = 0;
				rot_result = glm::slerp(start_rot, end_rot, rot_factor);
				break;
			}

		}
		skeleton->get_bone_at(i)->transform = glm::translate(glm::mat4(), pos_result) * glm::toMat4(rot_result) * glm::mat4();
	}

	skeleton->bone_transforms.resize(skeleton->size());
	skeleton->calculate_bone_positions(skeleton->get_first_bone());
	for (GLuint b = 0; b <skeleton->size(); b++)
	{
		skeleton->bone_transforms[b] = skeleton->get_bone_at(b)->final_transform * skeleton->get_bone_at(b)->offset_matrix;
	}
}

std::string AnimationController::type_name()
{
	return "AnimationController";
}

void AnimationController::set_speed(GLfloat anim_speed)
{

}

void AnimationController::attach_skeleton(Skeleton* skel)
{
	skeleton = skel;
}

void AnimationController::add_duration(GLuint d)
{
	durations.push_back(d);
}

void AnimationController::change_animation(GLuint index)
{
	if (current_animation != index && !(index >= durations.size()))
	{
		start_time = 0;
		elapsed_time = 0;
		current_animation = index;
	}
}