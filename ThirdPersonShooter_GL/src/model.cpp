#include "model.hpp"
#include "graphics.hpp"
#include "asset_loader.hpp"

void Model::set_meshes(std::vector<Mesh> data)
{
	//meshes = data;

}

void Model::create(std::string path, std::vector<std::string> anim_paths, AnimationController* animator)
{
	skeleton = Skeleton();
	AssetLoader::model_from_file(path.c_str(), this, anim_paths, animator);
	if (animator != nullptr)
		animator->attach_skeleton(&skeleton);
	skeleton.init();

	for (GLuint i = 0; i < meshes.size(); i++)
	{
		meshes[i].create();
	}

}

void Model::draw()
{
	GLuint model_location = glGetUniformLocation(Graphics::get_default_shader(), "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform->get_transformation()));

	GLuint bone_loc = glGetUniformLocation(Graphics::get_default_shader(), "bone_mats");
	glUniformMatrix4fv(bone_loc, skeleton.size(), GL_FALSE, glm::value_ptr(skeleton.bone_transforms[0]));

	for (GLuint i = 0; i < meshes.size(); i++)
	{
		Graphics::draw_mesh(&meshes[i]);
	}
}

GLuint Model::get_mesh_count()
{
	return meshes.size();
}

std::string Model::type_name()
{
	return "Model";
}

glm::vec3 Model::get_min_bounds()
{
	return meshes[0].get_min_bounds();
}

glm::vec3 Model::get_max_bounds()
{
	return meshes[0].get_max_bounds();
}