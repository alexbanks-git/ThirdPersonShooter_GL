#pragma once
#include <memory>
#include "GL/glew.h"
#include "controller_component.hpp"
#include "animation.hpp"

class AnimationController : public ControllerComponent
{
private:
	GLuint current_animation;
	std::vector<Animation> animations;
public:
	AnimationController(Entity* entity);
	/**
	 * @brief Updates the pose of the model attached to this animation controller.
	 */
	void update();

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Begins playback of a specific animation.
	 * @param index the index of the animation to play
	 * @param loop whether or not the animation should automatically repeat
	 */
	void play_animation(GLuint index, bool loop = false);

	/**
	 * @brief Adds an animation to this animation controller.
	 * @param animation the animation to add 
	 */
	void add_animation(Animation animation);

	/**
	 * @brief Returns the index of the currently playing animation.
	 * @returns the current animation's index
	 */
	GLuint get_active_animation();

	/**
	 * @brief Returns whether or not the current animation is playing.
	 * @returns true if current animation is playing, and false otherwise
	 */
	bool animation_playing();
};