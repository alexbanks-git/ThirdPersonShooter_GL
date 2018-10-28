#pragma once
#include <iostream>
#include <fstream>
#include "sstream"
#include <algorithm>
#include "GL\glew.h"
#include "SDL.h"
#include "model.hpp"
#include "shader.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

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
	 * @brief Draws a mesh to the screen.
	 * @param mesh the mesh to draw
	 */
	static void draw_mesh(Mesh* mesh);

	/**
	 * @brief Retrieves the time passed since the last update.
	 * @returns the time passed since the last update
	 */
	static GLfloat get_elapsed_time();

	/**
	 * @brief Returns the renderer
	 * @returns the renderer
	 */
	static SDL_Renderer* get_renderer();

	static void clear_screen(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	static glm::vec4 to_world_coords(glm::vec4 vec, glm::mat4 model);

	static void set_matrices(glm::mat4 p, glm::mat4 v);
};