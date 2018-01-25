#pragma once
#include <iostream>
#include <fstream>
#include "sstream"
#include <algorithm>
#include "GL\glew.h"
#include "SDL.h"
#include "model.hpp"

class Graphics
{
public:
	/**
	 * @brief Initializes SDL and GLEW, and sets the target frames per second.
	 * @param w the width of the SDL window
	 * @param h the height of the SDL window
	 * @param fps the target frames per second
	 * @returns 0 on error, or 1 otherwise
	 */
	static GLint init_graphics(GLint w, GLint h, GLfloat fps);

	/**
	 * @brief Swaps buffers and updates the game by one frame.
	 */
	static void update_window();

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

	/**
	 * @brief Retrieves the current window.
	 * @returns the SDL window
	 */
	static SDL_Window* get_window();

	/**
	* @brief Retrieves the width of the current window.
	* @returns the current window's width
	*/
	static GLint get_window_width();

	/**
	* @brief Retrieves the height of the current window.
	* @returns the current window's height
	*/
	static GLint get_window_height();

	/**
	 * @brief Retrieves the default shader.
	 * @returns the default shader
	 */
	static GLuint get_default_shader();

	/**
	* @brief Retrieves the user interface shader.
	* @returns the user interface shader
	*/
	static GLuint get_user_interface_shader();

	/**
	 * @brief Retrieves the skybox shader.
	 * @returns the skybox shader
	 */
	static GLuint get_skybox_shader();

	/**
	 * @brief Draws a mesh to the screen.
	 * @param mesh the mesh to draw
	 */
	static void Graphics::draw_mesh(Mesh* mesh);

	/**
	 * @brief Retrieves the debug shader.
	 * @returns the debug shader
	 */
	static GLuint get_debug_shader();

	/**
	 * @brief Retrieves the time passed since the last update.
	 * @returns the time passed since the last update
	 */
	static GLfloat get_elapsed_time();

	/**
	 * @brief Returns the image shader.
	 * @returns the image shader
	 */
	static GLuint Graphics::get_image_shader();

	/**
	 * @brief Returns the renderer
	 * @returns the renderer
	 */
	static SDL_Renderer* get_renderer();

	static void clear_screen(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
};