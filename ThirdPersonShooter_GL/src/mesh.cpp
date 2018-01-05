#include <string>
#include "graphics.hpp"
#include "mesh.hpp"

void Mesh::add_bone_weight(GLuint index, GLuint bone_index, GLfloat bone_weight)
{
	if (vertices[index].bone_count == 0)
	{
		vertices[index].bone_ids.x = bone_index;
		vertices[index].bone_weights.x = bone_weight;
	}
	else if (vertices[index].bone_count == 1)
	{
		vertices[index].bone_ids.y = bone_index;
		vertices[index].bone_weights.y = bone_weight;
	}
	else if (vertices[index].bone_count == 2)
	{
		vertices[index].bone_ids.z = bone_index;
		vertices[index].bone_weights.z = bone_weight;
	}
	else if (vertices[index].bone_count == 3)
	{
		vertices[index].bone_ids.w = bone_index;
		vertices[index].bone_weights.w = bone_weight;
	}
	vertices[index].bone_count++;
}

void Mesh::set_diffuse_texture(std::string path)
{
	diffuse_texture = path;
}
void Mesh::set_specular_texture(std::string path)
{
	specular_texture = path;
}

void Mesh::set_normal_map(std::string path)
{
	normal_map = path;
}

void Mesh::set_indices(std::vector<GLuint> data)
{
	indices = data;
}

void Mesh::set_vertices(std::vector<glm::vec3> data)
{
	if (vertices.size() <= 0)
		vertices.resize(data.size());

	for (GLuint i = 0; i < vertices.size(); i++)
	{
		vertices[i].position = data[i];
		vertices[i].bone_count = 0;
	}
}

void Mesh::set_normals(std::vector<glm::vec3> data)
{
	if (vertices.size() <= 0)
		vertices.resize(data.size());

	for (GLuint i = 0; i < vertices.size(); i++)
	{
		vertices[i].normal = data[i];
	}
}

void Mesh::set_tangents(std::vector<glm::vec3> data)
{
	if (vertices.size() <= 0)
		vertices.resize(data.size());

	for (GLuint i = 0; i < vertices.size(); i++)
	{
		vertices[i].tangent = data[i];
	}
}

void Mesh::set_bitangents(std::vector<glm::vec3> data)
{
	if (vertices.size() <= 0)
		vertices.resize(data.size());

	for (GLuint i = 0; i < vertices.size(); i++)
	{
		vertices[i].bitangent = data[i];
	}
}

void Mesh::set_texture_coordinates(std::vector<glm::vec2> data)
{
	if (vertices.size() <= 0)
		vertices.resize(data.size());

	for (GLuint i = 0; i < vertices.size(); i++)
	{
		vertices[i].uv = data[i];
	}
}

GLuint Mesh::get_index_count()
{
	return indices.size();
}

GLuint Mesh::get_vertex_count()
{
	return vertices.size();
}

void Mesh::bind()
{
	bind_texture(diffuse_id, "diffuse_texture", 0);
	bind_texture(specular_id, "specular_texture", 1);
	bind_texture(normal_id, "normal_map", 2);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(vao);
}

void Mesh::bind_texture(GLuint id, std::string location, GLuint num)
{
	glActiveTexture(GL_TEXTURE0+num);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(Graphics::get_default_shader(), location.c_str()), num);
}

void Mesh::create()
{
	glm::vec3 min = glm::vec3();
	glm::vec3 max = glm::vec3();

	for (GLuint i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].position.x > max.x)
		{
			max.x = vertices[i].position.x;
		}
		else if (vertices[i].position.x < min.x)
		{
			min.x = vertices[i].position.x;
		}
		if (vertices[i].position.y > max.y)
		{
			max.y = vertices[i].position.y;
		}
		else if (vertices[i].position.y < min.y)
		{
			min.y = vertices[i].position.y;
		}
		if (vertices[i].position.z > max.z)
		{
			max.z = vertices[i].position.z;
		}
		else if (vertices[i].position.z < min.z)
		{
			min.z = vertices[i].position.z;
		}
	}
	min_bounds = min;
	max_bounds = max;

	SDL_Surface* diffuse_surface = IMG_Load(diffuse_texture.c_str());
	create_texture(diffuse_surface, &diffuse_id);

	SDL_Surface* specular_surface = IMG_Load(specular_texture.c_str());
	create_texture(specular_surface, &specular_id);

	SDL_Surface* normal_surface = IMG_Load(normal_map.c_str());
	create_texture(normal_surface, &normal_id);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * (sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));

	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_UNSIGNED_INT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_ids));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_weights));

	glBindVertexArray(0);
}

glm::vec3 Mesh::get_min_bounds()
{
	return min_bounds;
}

glm::vec3 Mesh::get_max_bounds()
{
	return max_bounds;
}

void Mesh::create_texture(SDL_Surface* surface, GLuint* id)
{
	if (id == nullptr)
	{
		std::clog << "Mesh::create_texture: id is null" << std::endl;
		return;
	}

	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);

	if (surface == nullptr)
	{
		std::clog << "Mesh::create_texture: surface is null" << std::endl;
		glm::vec2 pixels = glm::vec2(0.0f, 0.0f);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, glm::value_ptr(pixels));
	}
	else
	{
		SDL_PixelFormat* pixel_format = surface->format;
		GLuint type;

		if (pixel_format->Amask)
			type = GL_RGBA;
		else
			type = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, type, surface->w, surface->h, 0, type, GL_UNSIGNED_BYTE, surface->pixels);

		SDL_FreeSurface(surface);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
