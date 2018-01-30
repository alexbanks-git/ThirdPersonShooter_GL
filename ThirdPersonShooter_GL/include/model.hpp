#pragma once
#include "drawable_component.hpp"
#include "mesh.hpp"
#include "skeleton.hpp"
#include "animation_controller.hpp"

class Model : public DrawableComponent
{
public:
	Model(Entity* entity);
	std::vector<Mesh> meshes;
	Skeleton skeleton;
	bool done;
	/**
	 * @brief Retrieves the number of meshes the model contains.
	 * @returns the number of meshes
	 */
	GLuint get_mesh_count();

	/**
	 * @brief Loads model from file, and buffers vertex information.
	 */
	void Model::create(std::string path, bool has_bones=false);

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