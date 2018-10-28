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

void Mesh::add_index(GLuint index)
{
	indices.push_back(index);
}

void Mesh::add_vertex(glm::vec3 vert)
{
	Vertex v = Vertex();
	v.position = vert;
	vertices.push_back(v);
	if (v.position.x > max_bounds.x)
	{
		max_bounds.x = v.position.x;
	}
	else if (v.position.x < min_bounds.x)
	{
		min_bounds.x = v.position.x;
	}
	if (v.position.y > max_bounds.y)
	{
		max_bounds.y = v.position.y;
	}
	else if (v.position.y < min_bounds.y)
	{
		min_bounds.y = v.position.y;
	}
	if (v.position.z > max_bounds.z)
	{
		max_bounds.z = v.position.z;
	}
	else if (v.position.z < min_bounds.z)
	{
		min_bounds.z = v.position.z;
	}
}

void Mesh::add_normal(glm::vec3 norm)
{
	vertices[vertices.size() - 1].normal = norm;
}

void Mesh::add_tangent(glm::vec3 tan)
{
	vertices[vertices.size() - 1].tangent = tan;
}

void Mesh::add_bitangent(glm::vec3 bitan)
{
	vertices[vertices.size() - 1].bitangent = bitan;
}

void Mesh::add_texture_coordinate(glm::vec2 tex)
{
	vertices[vertices.size() - 1].uv = tex;
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_id);
	glUniform1i(glGetUniformLocation(Shader::get_shader("default_shader"), "diffuse_texture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_id);
	glUniform1i(glGetUniformLocation(Shader::get_shader("default_shader"), "specular_texture"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_id);
	glUniform1i(glGetUniformLocation(Shader::get_shader("default_shader"), "normal_map"), 2);

	glBindVertexArray(vao);
}

void Mesh::bind_texture(GLuint id, std::string location, GLuint num)
{
	
}

void Mesh::create()
{
	
	SDL_Surface* diffuse_surface = IMG_Load(diffuse_texture.c_str());
	create_texture(diffuse_surface, &diffuse_id, "diffuse_texture", 0);

	SDL_Surface* specular_surface = IMG_Load(specular_texture.c_str());
	create_texture(specular_surface, &specular_id, "specular_texture", 1);

	SDL_Surface* normal_surface = IMG_Load(normal_map.c_str());
	create_texture(normal_surface, &normal_id, "normal_map", 2);

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

void Mesh::create_texture(SDL_Surface* surface, GLuint* id, std::string location, GLuint num)
{
	if (id == nullptr)
	{
		std::clog << "Mesh::create_texture: id is null" << std::endl;
		return;
	}

	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	GLubyte pixels[] = {0, 0, 0, 255};

	if (surface == nullptr)
	{
		std::clog << "Mesh::create_texture: surface is null" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	
}
