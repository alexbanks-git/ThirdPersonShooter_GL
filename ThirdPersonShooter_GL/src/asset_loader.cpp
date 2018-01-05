#include "asset_loader.hpp"
#include "SDL_image.h"

static const aiScene* scene;
void AssetLoader::model_from_file(std::string path, Model* model, std::vector<std::string> animation_paths, AnimationController* animator)
{
	Assimp::Importer importer;
	std::vector<Assimp::Importer> anim_importers;
	std::vector<const aiScene*> anim_scenes;
	anim_importers.resize(animation_paths.size());
	anim_scenes.resize(animation_paths.size());

	for (GLuint i = 0; i < animation_paths.size(); i++)
	{
		anim_scenes[i] = anim_importers[i].ReadFile(animation_paths[i], aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);
	}

	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	for (GLuint m = 0; m < scene->mNumMeshes; m++)
	{
		model->meshes.push_back(Mesh());
		std::vector<glm::vec2> texture_coordinates;
		std::vector<GLuint> indices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<glm::vec3> vertices;
		aiMatrix4x4 mat1 = aiMatrix4x4();

		if (model->skeleton.size() == 0 && animation_paths.size() > 0)
		{
			model->skeleton.increase_size();
			model->skeleton.get_first_bone()->parent_index = -1;
			setup_skeleton(scene->mRootNode->mChildren[0], &model->skeleton);
		}

		for (GLuint v = 0; v < scene->mMeshes[m]->mNumVertices; v++)
		{
			vertices.push_back(glm::vec3(
				scene->mMeshes[m]->mVertices[v].x,
				scene->mMeshes[m]->mVertices[v].y,
				scene->mMeshes[m]->mVertices[v].z));

			normals.push_back(glm::vec3(
				scene->mMeshes[m]->mNormals[v].x,
				scene->mMeshes[m]->mNormals[v].y,
				scene->mMeshes[m]->mNormals[v].z));

			tangents.push_back(glm::vec3(
				scene->mMeshes[m]->mTangents[v].x,
				scene->mMeshes[m]->mTangents[v].y,
				scene->mMeshes[m]->mTangents[v].z));

			bitangents.push_back(glm::vec3(
				scene->mMeshes[m]->mBitangents[v].x,
				scene->mMeshes[m]->mBitangents[v].y,
				scene->mMeshes[m]->mBitangents[v].z));

			texture_coordinates.push_back(glm::vec2(
				scene->mMeshes[m]->mTextureCoords[0][v].x, scene->mMeshes[m]->mTextureCoords[0][v].y));
		}
		model->meshes[m].set_vertices(vertices);

		for (GLuint b = 0; b < scene->mMeshes[m]->mNumBones; b++)
		{
			GLuint index = 0;
			for (GLuint i = 0; i < model->skeleton.size(); i++)
			{
				std::string string1 = scene->mMeshes[m]->mBones[b]->mName.C_Str();
				std::string string2 = model->skeleton.get_bone_at(i)->name.c_str();
				if (string1.compare(string2) == 0)
				{
					index = i;
					model->skeleton.get_bone_at(i)->offset_matrix = to_glm_mat(scene->mMeshes[m]->mBones[b]->mOffsetMatrix);
					break;
				}
			}

			for (GLuint w = 0; w < scene->mMeshes[m]->mBones[b]->mNumWeights; w++)
			{
				model->meshes[m].add_bone_weight(scene->mMeshes[m]->mBones[b]->mWeights[w].mVertexId,
					index, scene->mMeshes[m]->mBones[b]->mWeights[w].mWeight);
			}
		}
		
		for (GLuint a = 0; a < anim_scenes.size(); a++)
		{
			if (anim_scenes[a]->mNumAnimations == 0)
			{
				std::cout << animation_paths[a] << " does not contain animations." << std::endl;
				continue;
			}
			animator->add_duration(anim_scenes[a]->mAnimations[0]->mDuration);
			
			for (GLuint c = 0; c < anim_scenes[a]->mAnimations[0]->mNumChannels; c++)
			{

				for (GLuint p = 0; p < anim_scenes[a]->mAnimations[0]->mChannels[c]->mNumPositionKeys; p++)
				{
					glm::vec3 position;
					position.x = anim_scenes[a]->mAnimations[0]->mChannels[c]->mPositionKeys[p].mValue.x;
					position.y = anim_scenes[a]->mAnimations[0]->mChannels[c]->mPositionKeys[p].mValue.y;
					position.z = anim_scenes[a]->mAnimations[0]->mChannels[c]->mPositionKeys[p].mValue.z;

					GLfloat time = anim_scenes[a]->mAnimations[0]->mChannels[c]->mPositionKeys[p].mTime;

					model->skeleton.add_position_key(a, anim_scenes[a]->mAnimations[0]->mChannels[c]->mNodeName.C_Str(),
						time, position);
				}
				

				for (GLuint r = 0; r < anim_scenes[a]->mAnimations[0]->mChannels[c]->mNumRotationKeys; r++)
				{
					/*GLuint current_rot_animation = 0;
					if (start_frames[current_rot_animation + 1] / 24 >= (scene->mAnimations[0]->mChannels[c]->mRotationKeys[r].mTime / scene->mAnimations[0]->mTicksPerSecond))
						current_rot_animation++;*/
					glm::quat rotation;
					rotation.x = anim_scenes[a]->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.x;
					rotation.y = anim_scenes[a]->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.y;
					rotation.z = anim_scenes[a]->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.z;
					rotation.w = anim_scenes[a]->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.w;

					GLfloat time = anim_scenes[a]->mAnimations[0]->mChannels[c]->mRotationKeys[r].mTime;

					model->skeleton.add_rotation_key(a, anim_scenes[a]->mAnimations[0]->mChannels[c]->mNodeName.C_Str(), time, rotation);
					//std::cout << "Got_Here" << std::endl;
				}
			}
			
		}

		
		for (GLuint f = 0; f < scene->mMeshes[m]->mNumFaces; f++)
		{
			for (GLuint i = 0; i < scene->mMeshes[m]->mFaces[f].mNumIndices; i++)
			{
				indices.push_back(scene->mMeshes[m]->mFaces[f].mIndices[i]);
			}
		}

		aiString diffuse_name;
		aiString specular_name;
		aiString normal_name;
		scene->mMaterials[scene->mMeshes[m]->mMaterialIndex]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), diffuse_name);
		scene->mMaterials[scene->mMeshes[m]->mMaterialIndex]->Get(AI_MATKEY_TEXTURE_SPECULAR(0), specular_name);
		scene->mMaterials[scene->mMeshes[m]->mMaterialIndex]->Get(AI_MATKEY_TEXTURE_NORMALS(0), normal_name);
		std::string diffuse = diffuse_name.C_Str();
		std::string specular = specular_name.C_Str();
		std::string normal = normal_name.C_Str();

		model->meshes[m].set_bitangents(bitangents);
		model->meshes[m].set_tangents(tangents);
		model->meshes[m].set_normals(normals);
		model->meshes[m].set_texture_coordinates(texture_coordinates);
		model->meshes[m].set_normal_map(normal);
		model->meshes[m].set_specular_texture(specular);
		model->meshes[m].set_diffuse_texture(diffuse);

		model->meshes[m].set_indices(indices);
	}
}

void AssetLoader::setup_skeleton(aiNode* node, Skeleton* skeleton)
{
	if (node == nullptr)
	{
		std::clog << "AssetLoader::setup_skeleton: node is null" << std::endl;
		return;
	}

	if (skeleton == nullptr)
	{
		std::clog << "AssetLoader::setup_skeleton: skeleton is null" << std::endl;
		return;
	}
	skeleton->get_last_bone()->transform = to_glm_mat(node->mTransformation);
	skeleton->get_last_bone()->final_transform = skeleton->get_last_bone()->transform;
	skeleton->get_last_bone()->name.resize(node->mName.length);

	for (GLuint i = 0; i < node->mName.length; i++)
	{
		skeleton->get_last_bone()->name[i] = node->mName.C_Str()[i];
	}

	GLuint parent_index = skeleton->size() - 1;

	for (GLuint c = 0; c < node->mNumChildren; c++)
	{
		skeleton->increase_size();
		skeleton->get_last_bone()->parent_index = parent_index;
		skeleton->get_bone_at(parent_index)->children.push_back(skeleton->size() - 1);
		setup_skeleton(node->mChildren[c], skeleton);
	}
}

glm::mat4 AssetLoader::to_glm_mat(aiMatrix4x4 mat1)
{
	glm::mat4 mat2;
	mat2[0][0] = mat1[0][0];
	mat2[0][1] = mat1[1][0];
	mat2[0][2] = mat1[2][0];
	mat2[0][3] = mat1[3][0];

	mat2[1][0] = mat1[0][1];
	mat2[1][1] = mat1[1][1];
	mat2[1][2] = mat1[2][1];
	mat2[1][3] = mat1[3][1];

	mat2[2][0] = mat1[0][2];
	mat2[2][1] = mat1[1][2];
	mat2[2][2] = mat1[2][2];
	mat2[2][3] = mat1[3][2];

	mat2[3][0] = mat1[0][3];
	mat2[3][1] = mat1[1][3];
	mat2[3][2] = mat1[2][3];
	mat2[3][3] = mat1[3][3];
	return mat2;
}