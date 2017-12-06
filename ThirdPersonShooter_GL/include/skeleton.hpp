#pragma once
#include <vector>
#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/quaternion.hpp>

class Skeleton
{
private:
	std::vector<GLfloat> durations;

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

	typedef struct
	{
		glm::mat4 offset_matrix;
		glm::mat4 transform;
		glm::mat4 final_transform;
		std::string name;
		GLint parent_index;
		std::vector<GLuint> children;
		std::vector<std::vector<PositionKey>> position_keys;
		std::vector<std::vector<RotationKey>> rotation_keys;
	}Bone;

	std::vector<Bone> bones;
	GLuint current_animation = 0;

public:

	std::vector<glm::mat4> bone_transforms;

	/**
	 * @brief Recursively navigates the skeleton's bone hierarchy and calculates the transforms of bones.
	 * @param bone the current bone to calculate the transform of
	 */
	void calculate_bone_positions(Bone* bone);

	/**
	 * @brief Searches for a bone with the specified name.
	 * @param name the name of the bone to search for
	 * @returns the bone with the specified name
	 */
	Bone* bone_by_name(std::string name);

	/**
	 * @brief Adds a position key to a specific bone in the skeleton's bone hierarchy.
	 * @param index the index of the animation that the position key belongs to
	 * @param name the name of the bone to add the position key to
	 * @param time the time (in ticks) of the position key
	 * @param pos the position of the position key
	 */
	void add_position_key(GLuint index, std::string name, GLfloat time, glm::vec3 pos);

	/**
	 * @brief Adds a rotation key to a specific bone in the skeleton's bone hierarchy.
	 * @param index the index of the animation that the rotation key belongs to
	 * @param name the name of the bone to add the rotation key to
	 * @param time the time (in ticks) of the rotation key
	 * @param rot the rotation of the rotation key
	 */
	void add_rotation_key(GLuint index, std::string name, GLfloat time, glm::quat rot);

	/**
	 * @brief Adds an empty bone to the skeleton's bone hierarchy.
	 */
	void increase_size();

	/**
	 * @brief Retrieves the bone at the specified index
	 * @param i the index of the bone
	 * @returns the bone at the specified index
	 */
	Bone* get_bone_at(GLuint i);

	/**
	 * @brief Retrieves the last bone in the skeleton's bone hierarchy
	 * @returns the last bone in the hierarchy
	 */
	Bone* get_last_bone();

	/**
	 * @brief Retrieves the first bone in the skeleton's bone hierarchy
	 * @returns the first bone in the hierarchy
	 */
	Bone* get_first_bone();

	/**
	 * @brief Retrieves the size of the skeleton's bone hierarchy
	 * @returns the size of the hierarchy
	 */
	GLuint size();

	/**
	 * @brief Sets the final transforms of the skeleton's bones
	 */
	void init();
};