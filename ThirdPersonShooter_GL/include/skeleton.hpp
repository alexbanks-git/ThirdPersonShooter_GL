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
		glm::mat4 other_transform;
	}Bone;

	std::vector<Bone> bones;
	GLuint current_animation = 0;

public:
	glm::mat4 root;
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
	 * @brief Adds an empty bone to the skeleton's bone hierarchy.
	 */
	void increase_size();

	/**
	 * @brief Retrieves the bone at the specified index
	 * @param index the index of the bone
	 * @returns the bone at the specified index
	 */
	Bone* get_bone_at(GLuint index);

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

	/**
	 * @brief Rotates the bone at the specified index.
	 * @param bone_index the index of the bone
	 * @param angle the angle to rotate the bone by
	 * @param axis the axis to perform the rotation around
	 */
	void rotate_bone(GLuint bone_index, GLfloat angle, glm::vec3 axis);
};