#pragma once
#include <vector>
#include "model.hpp"

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
	static void recursive_func(aiNode* node, Skeleton* skeleton);
public:
	/**
	* @brief Loads a model from an OBJ or FBX file.
	* @param path the path of the file
	*/
	static void AssetLoader::model_from_file(std::string path, Model* model, std::vector<std::string> animation_paths);
};