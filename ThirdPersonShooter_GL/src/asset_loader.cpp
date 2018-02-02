#include <map>
#include <assimp/cimport.h>  
#include "asset_loader.hpp"
#include "SDL_image.h"

static const aiScene* scene;
static std::map<std::string, Model*> loaded_models;
static std::map<std::string, Animation> loaded_animations;

void AssetLoader::model_from_file(std::string path, Model* model, bool has_bones)
{
	Assimp::Importer importer;
	Model* temp;
	if ((temp = get_model(path)) != nullptr)
	{
		model->meshes = temp->meshes;
		model->skeleton = temp->skeleton;
		model->loaded = true;
		return;
	}
	std::cout << path << std::endl;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	if (model->skeleton.size() == 0 && has_bones)
	{
		model->skeleton.increase_size();
		model->skeleton.get_first_bone()->parent_index = -1;
		setup_bone_transforms(scene, scene->mRootNode, &model->skeleton);
	}

	setup_model(scene, scene->mRootNode, *model);
	loaded_models.insert(std::make_pair(path, model));
}

void AssetLoader::setup_model(const aiScene* scene, aiNode* node, Model& model)
{
	if (node->mNumMeshes > 0)
	{
		for (GLint m = 0; m < node->mNumMeshes; m++)
		{
			model.meshes.push_back(Mesh());
			setup_mesh(scene, scene->mMeshes[node->mMeshes[m]], model, model.meshes[model.meshes.size() - 1]);
		}
	}

	for (GLint c = 0; c < node->mNumChildren; c++)
	{
		setup_model(scene, node->mChildren[c], model);
	}
}

void AssetLoader::setup_skinning_info(const aiScene* scene, aiBone* ai_bone, Mesh& mesh, Skeleton& skeleton)
{
	GLuint index = 0;
	for (GLuint i = 0; i < skeleton.size(); i++)
	{
		std::string string1 = ai_bone->mName.C_Str();
		std::string string2 = skeleton.get_bone_at(i)->name.c_str();
		if (string1.compare(string2) == 0)
		{
			index = i;
			skeleton.get_bone_at(i)->offset_matrix = to_glm_mat(ai_bone->mOffsetMatrix);
			skeleton.get_bone_at(i)->offset_matrix[3][0] /= 100.0f;
			skeleton.get_bone_at(i)->offset_matrix[3][1] /= 100.0f;
			skeleton.get_bone_at(i)->offset_matrix[3][2] /= 100.0f;;
			break;
		}
	}

	for (GLuint w = 0; w < ai_bone->mNumWeights; w++)
	{
		mesh.add_bone_weight(ai_bone->mWeights[w].mVertexId,
			index, ai_bone->mWeights[w].mWeight);
	}
}

Model* AssetLoader::get_model(std::string name)
{
	std::map<std::string, Model*>::iterator it;
	if ((it = loaded_models.find(name)) != loaded_models.end())
	{
		return it->second;
	}
	return nullptr;
}

Animation AssetLoader::get_animation(std::string name)
{
	std::map<std::string, Animation>::iterator it;
	Animation d = Animation(new Skeleton());
	if ((it = loaded_animations.find(name)) != loaded_animations.end())
	{
		d = it->second;
	}
	return d;
}

bool AssetLoader::anim_exists(std::string name)
{
	std::map<std::string, Animation>::iterator it;
	if ((it = loaded_animations.find(name)) != loaded_animations.end())
	{
		return true;
	}
	return false;
}

void AssetLoader::setup_mesh(const aiScene* scene, aiMesh* ai_mesh, Model& model, Mesh& mesh)
{
	std::vector<glm::vec2> texture_coordinates;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<glm::vec3> vertices;

	for (GLint v = 0; v < ai_mesh->mNumVertices; v++)
	{
		mesh.add_vertex(glm::vec3(
			ai_mesh->mVertices[v].x / 100.0f,
			ai_mesh->mVertices[v].y / 100.0f,
			ai_mesh->mVertices[v].z / 100.0f));

		mesh.add_normal(glm::vec3(
			ai_mesh->mNormals[v].x,
			ai_mesh->mNormals[v].y,
			ai_mesh->mNormals[v].z));

		mesh.add_tangent(glm::vec3(
			ai_mesh->mTangents[v].x,
			ai_mesh->mTangents[v].y,
			ai_mesh->mTangents[v].z));

		mesh.add_bitangent(glm::vec3(
			ai_mesh->mBitangents[v].x,
			ai_mesh->mBitangents[v].y,
			ai_mesh->mBitangents[v].z));

		mesh.add_texture_coordinate(glm::vec2(
			ai_mesh->mTextureCoords[0][v].x, ai_mesh->mTextureCoords[0][v].y));
	}

	for (GLuint b = 0; b < ai_mesh->mNumBones; b++)
	{
		setup_skinning_info(scene, ai_mesh->mBones[b], mesh, model.skeleton);
	}

	for (GLuint f = 0; f < ai_mesh->mNumFaces; f++)
	{
		for (GLuint i = 0; i < ai_mesh->mFaces[f].mNumIndices; i++)
		{
			mesh.add_index(ai_mesh->mFaces[f].mIndices[i]);
		}
	}

	aiString diffuse_name;
	aiString specular_name;
	aiString normal_name;
	scene->mMaterials[ai_mesh->mMaterialIndex]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), diffuse_name);
	scene->mMaterials[ai_mesh->mMaterialIndex]->Get(AI_MATKEY_TEXTURE_SPECULAR(0), specular_name);
	scene->mMaterials[ai_mesh->mMaterialIndex]->Get(AI_MATKEY_TEXTURE_NORMALS(0), normal_name);
	std::string diffuse = diffuse_name.C_Str();
	std::string specular = specular_name.C_Str();
	std::string normal = normal_name.C_Str();

	mesh.set_normal_map(normal);
	mesh.set_specular_texture(specular);
	mesh.set_diffuse_texture(diffuse);
}

void AssetLoader::setup_bone_transforms(const aiScene* scene, aiNode* node, Skeleton* skeleton)
{
	if (node == nullptr)
	{
		std::clog << "AssetLoader::setup_bone_transforms: node is null" << std::endl;
		return;
	}

	if (skeleton == nullptr)
	{
		std::clog << "AssetLoader::setup_bone_transforms: skeleton is null" << std::endl;
		return;
	}
	skeleton->get_last_bone()->transform = to_glm_mat(node->mTransformation);
	skeleton->get_last_bone()->transform[3][0] /= 100.0f;
	skeleton->get_last_bone()->transform[3][1] /= 100.0f;
	skeleton->get_last_bone()->transform[3][2] /= 100.0f;
	skeleton->get_last_bone()->final_transform = skeleton->get_last_bone()->transform;
	skeleton->get_last_bone()->final_transform[3][0] /= 100.0f;
	skeleton->get_last_bone()->final_transform[3][1] /= 100.0f;
	skeleton->get_last_bone()->final_transform[3][2] /= 100.0f;
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
		setup_bone_transforms(scene, node->mChildren[c], skeleton);
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

void AssetLoader::load_animation(std::string path, Animation& animation)
{
	if (anim_exists(path))
	{
		Animation anim = get_animation(path);
		animation.copy(&anim);
		return;
	}
	Assimp::Importer importer = Assimp::Importer();
	const aiScene* anim_scene = importer.ReadFile(path, aiProcess_LimitBoneWeights);
	animation.set_duration(anim_scene->mAnimations[0]->mDuration);
	animation.set_path(path);

	for (GLuint c = 0; c < anim_scene->mAnimations[0]->mNumChannels; c++)
	{
		for (GLuint p = 0; p < anim_scene->mAnimations[0]->mChannels[c]->mNumPositionKeys; p++)
		{
			glm::vec3 position;
			position.x = anim_scene->mAnimations[0]->mChannels[c]->mPositionKeys[p].mValue.x / 100.0f;
			position.y = anim_scene->mAnimations[0]->mChannels[c]->mPositionKeys[p].mValue.y / 100.0f;
			position.z = anim_scene->mAnimations[0]->mChannels[c]->mPositionKeys[p].mValue.z / 100.0f;

			GLfloat time = anim_scene->mAnimations[0]->mChannels[c]->mPositionKeys[p].mTime;

			animation.add_position_key(anim_scene->mAnimations[0]->mChannels[c]->mNodeName.C_Str(),
				time, position);
		}


		for (GLuint r = 0; r < anim_scene->mAnimations[0]->mChannels[c]->mNumRotationKeys; r++)
		{
			glm::quat rotation;
			rotation.x = anim_scene->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.x;
			rotation.y = anim_scene->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.y;
			rotation.z = anim_scene->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.z;
			rotation.w = anim_scene->mAnimations[0]->mChannels[c]->mRotationKeys[r].mValue.w;

			GLfloat time = anim_scene->mAnimations[0]->mChannels[c]->mRotationKeys[r].mTime;

			animation.add_rotation_key(anim_scene->mAnimations[0]->mChannels[c]->mNodeName.C_Str(), time, rotation);
		}
	}
	
	loaded_animations.insert(std::make_pair(path, animation));
}