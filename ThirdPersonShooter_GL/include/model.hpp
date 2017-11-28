#pragma once
#include "drawable_component.hpp"
#include "mesh.hpp"
#include "skeleton.hpp"

class Model : public DrawableComponent
{
public:
	std::vector<Mesh> meshes;
	Skeleton skeleton;
	/**
	 * @brief Retrieves the number of meshes the model contains.
	 * @param returns the number of meshes
	 */
	GLuint get_mesh_count();

	/**
	 * @brief Loads model from file, and buffers vertex information.
	 * @param path the file path of the model
	 */
	void create(std::string path);

	/**
	 * @brief Draws all meshes this model contains.
	 */
	void draw();

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Sets the meshes for the model.
	 * @param data the mesh data
	 */
	void set_meshes(std::vector<Mesh> data);

	/**
	 * @brief Retrieves the smallest X, Y, and Z values of the model.
	 * @returns a 3D vector containing the smallest X, Y, and Z values
	 */
	glm::vec3 get_min_bounds();

	/**
	 * @brief Retrieves the largest X, Y, and Z values of the model.
	 * @returns a 3D vector containing the largest X, Y, and Z values
	 */
	glm::vec3 get_max_bounds();
};