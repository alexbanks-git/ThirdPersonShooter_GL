#include <iostream>
#include "animation_controller.hpp"
#include "SDL.h"
#include "entity.hpp"

AnimationController::AnimationController(Entity* entity) : ControllerComponent(entity)
{
	current_animation = -1;
}

void AnimationController::add_animation(Animation animation)
{
	animations.push_back(animation);
}

void AnimationController::update()
{
	if (current_animation != -1)
	{
		animations[current_animation].update();
		if (animations[current_animation].has_constraints())
		{
			glm::vec3 delta = animations[current_animation].get_delta_position();
			glm::vec3 final_delta = delta;

			if (delta.y > 0)
				final_delta.y = delta.y*8.0f;

			final_delta.x *= -1.0f;
			final_delta.z *= -1.0f;

			owner.get_component<PhysicsBody>()->apply_impulse(Transform::world_up_vector() * final_delta.y);
			owner.get_component<PhysicsBody>()->apply_impulse(transform.forward * final_delta.z);
			owner.get_component<PhysicsBody>()->apply_impulse(transform.right * final_delta.x);
		}
	}
}

std::string AnimationController::type_name()
{
	return "AnimationController";
}

void AnimationController::play_animation(GLuint index, bool loop)
{
	if (index < animations.size())
	{
		if (!animations[current_animation].is_playing())
		{
			current_animation = index;
			animations[current_animation].play(loop);
		}
		else if (animations[current_animation].is_looping() && (index != current_animation))
		{
			animations[current_animation].stop();
			current_animation = index;
			animations[current_animation].play(loop);
		}

	}
}

GLuint AnimationController::get_active_animation()
{
	return current_animation;
}

bool AnimationController::animation_playing()
{
	return animations[current_animation].is_playing();
}