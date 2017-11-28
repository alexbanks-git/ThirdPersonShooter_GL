#pragma once
#include <iostream>
#include <vector>
#include "SDL_image.h"
#include "GL/glew.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/quaternion.hpp>

class Mesh
{
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint diffuse_id;
	GLuint specular_id;
	GLuint normal_id;
	std::string normal_map;
	std::string specular_texture;
	std::string diffuse_texture;
	std::vector<GLuint> indices;
	glm::vec3 min_bounds;
	glm::vec3 max_bounds;

	typedef struct
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 uv;
		glm::vec4 bone_weights;
		glm::uvec4 bone_ids;
		GLuint bone_count = 0;
	}Vertex;

	std::vector<Vertex> vertices;

public:

	/**
	 * @brief Sets the positions for the mesh's vertices
	 * @param data the data to set the vertex positions with
	 */
	void set_vertices(std::vector<glm::vec3> data);

	/**
	 * @brief Sets the normals for the mesh's vertices
	 * @param data the data to set the vertex normals with
	 */
	void set_normals(std::vector<glm::vec3> data);
	/**
	 * @brief Sets the tangents for the mesh's vertices
	 * @param data the data to set the vertex tangents with
	 */
	void set_tangents(std::vector<glm::vec3> data);

	/**
	 * @brief Sets the bitangents for the mesh's vertices
	 * @param data the data to set the vertex bitangents with
	 */
	void set_bitangents(std::vector<glm::vec3> data);

	/**
	 * @brief Sets the texture coordinates for the mesh's vertices
	 * @param data the data to set the vertex texture coordinates with
	 */
	void set_texture_coordinates(std::vector<glm::vec2> data);

	/**
	 * @brief Sets the diffuse texture of the mesh.
	 * @param path the diffuse texture of the mesh
	 */
	void set_diffuse_texture(std::string path);

	/**
	* @brief Sets the specular texture of the mesh.
	* @param path the specular texture of the mesh
	*/
	void set_specular_texture(std::string path);

	/**
	* @brief Sets the normal map of the mesh.
	* @param path the normal map of the mesh
	*/
	void set_normal_map(std::string path);

	/**
	 * @brief Sets the indices for the mesh.
	 * @param data the to set the indices with
	 */
	void set_indices(std::vector<GLuint> data);

	/**
	 * @brief Generates, and binds, array and buffer objects for the mesh.
	 */
	void create();

	/**
	 * @brief Retrieves the number of indices for the mesh.
	 * @returns the number of indices
	 */
	GLuint get_index_count();

	/**
	 * @brief Retrieves the number of vertices for the mesh.
	 * @returns the number of vertices
	 */
	GLuint get_vertex_count();

	/**
	 * @brief Retrieves the smallest X, Y, and Z coordinates.
	 * @returns 3D vector containing smallest X, Y, and Z coordinates
	 */
	glm::vec3 get_min_bounds();

	/**
	 * @brief Retrieves the max X, Y, and Z coordinates.
	 * @returns 3D vector containing max X, Y, and Z coordinates
	 */
	glm::vec3 get_max_bounds();

	/**
	 * @brief Binds the mesh's vertex array object
	 */
	void bind();

	/**
	 * @brief Adds a bone weight for the vertex at the specified index
	 * @param index the index of the vertex
	 * @param bone_index the id of the bone that the bone weight is associated with
	 * @param bone_weight the bone weight to add
	 */
	void add_bone_weight(GLuint index, GLuint bone_index, GLfloat bone_weight);
};