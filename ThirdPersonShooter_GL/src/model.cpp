#include "model.hpp"
#include "graphics.hpp"
#include "asset_loader.hpp"

Model::Model(Entity* entity) : DrawableComponent(entity)
{
	material = 0;
}

void Model::set_meshes(std::vector<Mesh> data)
{

}

void Model::create(std::string path, bool has_bones)
{
	skeleton = Skeleton();

	AssetLoader::model_from_file(path.c_str(), this, has_bones);

	if (!loaded)
	{
		if (has_bones)
		{
			skeleton.init();
		}

		for (GLuint i = 0; i < meshes.size(); i++)
		{
			meshes[i].create();
		}
		loaded = true;
	}
}

void Model::draw()
{
	GLuint model_location = glGetUniformLocation(Graphics::get_default_shader(), "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform.get_transformation()));

	if (skeleton.size() > 0)
	{
		GLuint bone_loc = glGetUniformLocation(Graphics::get_default_shader(), "bone_mats");
		glUniformMatrix4fv(bone_loc, skeleton.size(), GL_FALSE, glm::value_ptr(skeleton.bone_transforms[0]));

		GLint flag_loc = glGetUniformLocation(Graphics::get_default_shader(), "has_bones");
		glUniform1i(flag_loc, 1);
	}
	else
	{
		GLint flag_loc = glGetUniformLocation(Graphics::get_default_shader(), "has_bones");
		glUniform1i(flag_loc, 0);
	}

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