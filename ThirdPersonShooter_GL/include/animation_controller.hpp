#pragma once
#include "GL/glew.h"
#include "controller_component.hpp"
#include "skeleton.hpp"

class AnimationController : public ControllerComponent
{
private:
	Skeleton* skeleton;
	GLuint start_time;
	std::vector<GLuint> durations;
	GLuint current_animation;
	GLuint elapsed_time;

public:
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
	 * @brief Sets the speed (in seconds) of the animation.
	 * @param anim_speed the speed of the animation
	 */
	void set_speed(GLfloat anim_speed);

	/**
	 * @brief Attaches a skeleton to this animation controller.
	 * @param skeleton the skeleton to attach
	 */
	void attach_skeleton(Skeleton* skel);

	/**
	 * @brief Sets the durations of the animations.
	 * @param d the duration to add to the list of durations
	 */
	void add_duration(GLuint d);

	/**
	 * @brief Changes the current animation to the animation of the given index.
	 * @param index the index of the animationto change to
	 */
	void change_animation(GLuint index);
};