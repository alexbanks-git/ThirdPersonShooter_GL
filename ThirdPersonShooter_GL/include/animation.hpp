#pragma once
#include <vector>
#include <map>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "skeleton.hpp"

class Animation
{
private:
	typedef struct
	{
		GLfloat time;
		glm::vec3 position;
	}PositionKey;

	typedef struct
	{
		GLfloat time;
		glm::quat rotation;
	}RotationKey;

	std::map<std::string, std::vector<PositionKey>> position_keys;
	std::map<std::string, std::vector<RotationKey>> rotation_keys;
	glm::vec3 axis_constraint;
	std::string root_bone;
	GLfloat duration;
	bool playing;
	bool looping;
	std::string path;
	GLfloat start_time;
	GLfloat elapsed_time;
	Skeleton* skeleton;
	bool first_frame_passed;
	bool constraints_active;
	glm::vec3 delta_position;
	glm::vec3 prev_pos;
	glm::vec3 new_pos;
public:
	Animation(Skeleton* skel);

	/**
	 * @brief Sets a keyframe's position for a specific bone.
	 * @param bone_name the name of the bone
	 * @param time the time that the keyframe starts
	 * @param position the position of the bone during this keyframe
	 */
	void add_position_key(std::string bone_name, GLfloat time, glm::vec3 position);

	/**
	 * @brief Sets a keyframe's rotation for a specific bone.
	 * @param bone_name the name of the bone
	 * @param time the time that the keyframe starts
	 * @param rotation the rotation of the bone during this keyframe
	 */
	void add_rotation_key(std::string bone_name, GLfloat time, glm::quat rotation);

	/**
	 * @brief Sets the file path of this animation.
	 * @param file_path 
	 */
	void set_path(std::string file_path);

	/**
	 * @brief Sets the length of this animation.
	 * @param length
	 */
	void set_duration(GLfloat length);

	/**
	 * @brief Begins playback of the animation.
	 * @param loop whether or not this animation should repeat
	 */
	void play(bool loop=false);

	/**
	 * @brief Ends playback of the animation.
	 */
	void stop();

	/**
	 * @brief Updates the skeleton attached to this animation.
	 */
	void update();

	/**
	 * @brief Returns whether or not this animation is currently playing.
	 * @returns true if animation is playing, or false otherwise
	 */
	bool is_playing();

	/**
	 * @brief Returns whether or not this animation automatically repeats.
	 * @returns true is animation loops, or false otherwise
	 */
	bool is_looping();

	/**
	 * @brief Calculates the interpolated position between two keyframes based on elapsed time.
	 * @param start_key the first keyframe
	 * @param end_key the ending keyframe
	 * @returns the calculated position
	 */
	glm::vec3 interpolate_position(PositionKey start_key, PositionKey end_key);

	/**
	 * @brief Calculates the interpolated rotation between two keyframes based on elapsed time.
	 * @param start_key the first keyframe
	 * @param end_key the ending keyframe
	 * @returns the calculated rotation
	 */
	glm::quat interpolate_rotation(RotationKey start_key, RotationKey end_key);

	/**
	 * @brief Sets the bone that drives the world space position of the parent object.
	 * @param bone_name the bone name
	 */
	void set_root_bone(std::string bone_name);

	/**
	 * @brief Constrains the world space movement to specific axes.
	 * @param axis the axes to constrain movement to
	 */
	void constrain_axes(glm::vec3 axes);

	/**
	 * @brief Returns the change in position of the root bone.
	 * @preturns the change in position
	 */
	glm::vec3 get_delta_position();

	/**
	 * @brief Returns whether or not the animation has contraints
	 * @returns true if animation has constraints, or false otherwise
	 */
	bool has_constraints();
};