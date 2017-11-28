#pragma once
#include "GL/glew.h"
#include "controller_component.hpp"

class AnimationController : public ControllerComponent
{
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
};