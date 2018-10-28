#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "GL/glew.h"

class Shader
{
public:
	GLuint get_id();

	static void init();
	
	static GLuint get_shader(std::string shader_name);
	static void use_shader(std::string shader_name);
	static GLuint get_current_shader();

private:
	GLuint id;
	bool depth_test;
	bool culling;
	GLenum culling_mode;

	void set_depth_testing(bool b);
	void set_culling(bool b);
	void set_culling_mode(GLenum cull_mode);

	/**
	* @brief Creates a shader object.
	* @param shader_type the type of shader to be created
	* @param path the file path of the shader file
	* @returns the shader object if successful, or 0 on error
	*/
	static GLuint create_shader(std::string shader_type, std::string path);

	/**
	* @brief Creates a shader program.
	* @param vs_path the file path of the vertex shader
	* @param fs_path the file path of the fragment shader
	* @returns returns the shader program
	*/
	static GLuint create_shader_program(std::string vs_path, std::string fs_path);
};