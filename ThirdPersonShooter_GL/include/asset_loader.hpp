#pragma once
#include <memory>
#include <vector>
#include "model.hpp"
#include "animation.hpp"
#include "animation_controller.hpp"

class AssetLoader
{
private:
	/**
	 * @brief Converts an Assimp matrix into a GLM matrix
	 * @param mat1 the Assimp matrix to convert
	 * @returns the GLM matrix
	 */
	static glm::mat4 to_glm_mat(aiMatrix4x4 mat1);

	/**
	 * @brief Recursively iterates through a node's hierarchy and initializes a skeleton based on the data
	 * @param node the base node of the hierarchy
	 * @param skeleton the skeleton to initialize
	 */
	static void setup_bone_transforms(aiNode* node, Skeleton* skeleton);

	/**
	 * @brief Iterates through a node's children and sets up model data.
	 * @param node the parent node
	 * @param model the model to setup
	 */
	static void setup_model(aiNode* node, Model& model);

	/**
	 * @brief Sets a specific mesh's data based on Assimp mesh.
	 * @param ai_mesh the Assimp mesh
	 * @param model the model that the mesh belongs to
	 * @param mesh the mesh to setup
	 */
	static void setup_mesh(aiMesh* ai_mesh, Model& model, Mesh& mesh);

	/**
	 * @brief Initializes bone weights and offset matrices pertaining to a specific bone.
	 * @param ai_bone the Assimp bone to retrieve info from
	 * @param mesh the mesh that the skeleton belongs to
	 * @param skeleton the skeleton that the bone belongs to
	 */
	static void setup_skinning_info(aiBone* ai_bone, Mesh& mesh, Skeleton& skeleton);
public:
	/**
	* @brief Loads a model from an FBX file.
	* @param path the path of the file
	* @param animation_paths the paths to the animations to be used for the model
	* @param animator the animation controller attached to the entity that the model belongs to
	*/
	static void model_from_file(std::string path, Model* model, bool has_bones = false);

	/**
	 * @brief Loads an animation from an FBX file.
	 * @param path the file path of the animation
	 * @param animation the animation object to write data into
	 */
	static void load_animation(std::string path, Animation& animation);
};